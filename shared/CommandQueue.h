/* Panther3D Engine © 2004-2010 Reversity Studios (www.reversity.org)
* This contents may be used and/or copied only with the written permission or
* terms and conditions stipulated in agreement/contract.
-----------------------------------------------------------------------------
History: 
 08/27/2009 File created - kexik
*/
#pragma once

#include "tArray.h"

namespace P3D{

// Renderer command queue identifiers
enum 
{
	CQI_NONE=0,

	// --- Engine -> Renderer ---
	CQI_ENGINE2RENDERER_CANVAS_CREATE,
	CQI_ENGINE2RENDERER_CANVAS_UPDATE,
	CQI_ENGINE2RENDERER_CANVAS_DELETE,

	CQI_ENGINE2RENDERER_TEXTURE_CREATE,
	CQI_ENGINE2RENDERER_TEXTURE_UPDATE,
	CQI_ENGINE2RENDERER_TEXTURE_DELETE,

	CQI_ENGINE2RENDERER_VERTEX_BUFFER_CREATE,
	CQI_ENGINE2RENDERER_VERTEX_BUFFER_UPDATE, // Updates dynamic buffer of delete and create new static with given ID
	CQI_ENGINE2RENDERER_VERTEX_BUFFER_DELETE,

	CQI_ENGINE2RENDERER_INDEX_BUFFER_CREATE,
	CQI_ENGINE2RENDERER_INDEX_BUFFER_UPDATE,
	CQI_ENGINE2RENDERER_INDEX_BUFFER_DELETE,

	CQI_ENGINE2RENDERER_SHADER_CREATE,
	CQI_ENGINE2RENDERER_SHADER_DELETE,

	//CQI_RENDERER_INSTANCED_DATA,
	//CQI_RENDERER_MATERIAL_DATA,
	CQI_ENGINE2RENDERER_DRAW_CALL,

	// --- Renderer -> Engine ---
	CQI_RENDERER2ENGINE_GENERAL_REPLY,
	CQI_RENDERER2ENGINE_TEXTURE_LOAD_REQUEST,

};

struct sE2RCanvasDesc
{
	void* handle;
	Vec2i size;
	bool windowed;
};

struct sR2EGeneralReply
{
	ID id;
	bool ok;
};

/// Interface wrapping command queue back way message notifications
class IQueueCommandReceiver
{
public:
	virtual void OnMessageReceived(unsigned int commandQueueIdentifier, const void* &outPointer)=0;
};


/// Maximum byte length of command structure
/** Also determines queue memory footprint as thisNumber * 4 (on 32bit systems) or 
thisNumber * 8 (on x64) is always allocated on queue's creation.
*/
#define CQ_MAX_STRUCT_LENGTH 8192 /* 8kB */

/**
Queue for various types of commands useful for thread-thread communication.
Every command is identified with command type (integer value) and has assigned 
structure containing command data. This is very effective way to store structures
with size up to CQ_MAX_STRUCT_LENGTH. Queue takes extra memory based on
CQ_MAX_STRUCT_LENGTH and also extra 20 bytes (on 32bit) for each item in queue.
*/
class CCommandQueue
{
private:
	/// One Item of CCommandQueue
	struct sQueueItem
	{
		/// Command type identifier
		unsigned int commandQueueIdentifier;
		/// Memory containing command structure
		void* memory;
		/// Size of memory in bytes
		unsigned int memorySize;
		/// Receiver which will get response for this command
		IQueueCommandReceiver* receiver;
		/// Command "instance" identifier, will be copied to response
		unsigned int ticketID;
	};
public:
	inline CCommandQueue()
	{
		m_idx = 0;
		m_bDequeuing = false;

		for(unsigned int i=0; i<CQ_MAX_STRUCT_LENGTH;i++)
			m_cache[i]=NULL;
	}

	inline ~CCommandQueue()
	{
		// delete memory in queue
		for(unsigned int i=0; i<m_items.Size();i++)
			if (m_items[i].memory) free(m_items[i].memory);

		// delete memory in cache list
		for(unsigned int i=0; i<CQ_MAX_STRUCT_LENGTH;i++)
		{
			void** ptr;
			while(ptr = m_cache[i].Remove())
				free(*ptr);
		}
	}

	/** Enqueues new command
	Queue must be emptied (using Dequeue) before enqueuing new items!
	<br>
	<br>
	Example of enqueuing new item:<br>
	sCRendererCreateTexture *cmd; // NULL is important here
	queue.Enqueue(this, 12, CQI_RENDERER_CREATE_TEXTURE, (void*&)cmd, sizeof(sCRendererCreateTexture));
	cmd->param = sth;
	\param receiver Pointer to class implementing CQueueCommandReceiver interface (will get response for this command)
	\param commandQueueIdentifier Identifier of command item
	\param inOutPointer Pointer to command structure see remarks
	\param memSizeInBytes Struct memory size in bytes
	\remarks DON'T DELETE RETURNED MEMORY FOR STRUCT, COMMAND QUEUE WILL DO IT FOR YOU!
	\warning No destructors will be called on command structures
	*/
	inline void Enqueue(IQueueCommandReceiver* receiver, unsigned int commandQueueIdentifier, void* &inOutPointer, unsigned int memSizeInBytes)
	{
		if (m_bDequeuing && m_idx!=0 )
		{
			assertd(false, "You can't enqueue while queue is not dequeued fully!");
		}
		
		m_bDequeuing = false;

		assertd(memSizeInBytes<CQ_MAX_STRUCT_LENGTH, "CommandQueue cache: Trying to use structure longer than CQ_MAX_STRUCT_LENGTH!");
		
		// try to find free memory (with size enough to store struct for this command type)
		void* mem; void** ptr = m_cache[memSizeInBytes].Remove();
		if (ptr)
			mem = *ptr;
		else
		{
			// free memory not found, allocate it
			mem = malloc( memSizeInBytes );
		};

		// add new queue item to list
		sQueueItem &qi = m_items.AddEx();
		qi.commandQueueIdentifier = commandQueueIdentifier;
		qi.memory = mem;
		qi.memorySize = memSizeInBytes;
		qi.receiver = receiver;
		inOutPointer = mem;
	}

	/// Returns next item in command queue
	/**
	Will try to find next command in queue and return it. If command found, true will be returned and you can
	use commandQueueIdentifier and outPointer for processing, otherwise false - don't process command.
	\warning When Dequeue returns false don't use returned data and also don't call Dequeue again!
	\return Will return true if command found, false if no remaining commands left in queue.
	*/
	inline bool Dequeue(unsigned int &commandQueueIdentifier, const void* &outPointer, IQueueCommandReceiver* &outReceiver)
	{
		if (m_idx==m_items.Size()) // dequeued fully, restore to enqueue mode
		{
			m_items.Clear();
			m_bDequeuing = false;
			m_idx = 0;

			return false; // no more to dequeue
		}
		m_bDequeuing = true;

		outPointer = m_items[m_idx].memory;
		commandQueueIdentifier = m_items[m_idx].commandQueueIdentifier;

		// add to cache list based on memory size
		m_cache[ m_items[m_idx].memorySize ].AddEx( m_items[m_idx].memory );

		// call receiver
		//if ( m_items[m_idx].receiver ) m_items[m_idx].receiver->OnMessageReceived( commandQueueIdentifier, outPointer );
		outReceiver = m_items[m_idx].receiver;

		m_idx++;
		return true;
	}

	inline bool IsDequeuedFully()
	{
		if (m_bDequeuing == false && m_idx==0) 
			return true;
		else
			return false;
	}

private:
	tArray<sQueueItem> m_items;
	/*static */tArray<void*> m_cache[CQ_MAX_STRUCT_LENGTH];

	unsigned int m_idx; // current position starting from 0
	bool m_bDequeuing; // just assertion
};


class CDoubleCommandQueue
{
public:
	CDoubleCommandQueue(){
		m_qEngine = &m_queues[0];
		m_qModule = &m_queues[1];
	};
	/// Swaps engine and module queues
	inline void Swap()
	{
		assertd(m_qModule->IsDequeuedFully(), "Cannot Swap() CDoubleCommandBuffer while queue for dequeuing isn't dequeued fully!!");
		CCommandQueue* tmp = m_qEngine;
		m_qEngine = m_qModule;
		m_qModule = tmp;
	}
	/** Returns engine queue. Engine should only dequeue items from it and module enqueue items to it (back side of communication). */
	inline CCommandQueue* EngineQueue(){ return m_qEngine; }
	/** Returns module queue. Module should only dequeue items from it and enqine enqueue items to it (back side of communication). */
	inline CCommandQueue* ModuleQueue(){ return m_qModule; }

private:
	CCommandQueue m_queues[2];
	CCommandQueue* m_qEngine;
	CCommandQueue* m_qModule;
};

}; // ns