#ifndef __MARACAS_H__
#define __MARACAS_H__

#include <stdlib.h>
#include <stdio.h>

#define STATEMENT(...) do { __VA_ARGS__ } while(0)

//CONFIG
#define MRC_DEBUG_MOD
#define MRC_OUTPUT stdout

#ifdef MRC_DEBUG_MOD
	#define MRC_DEBUG_ONLY(x)	x
	#define MRC_PROMPT		"[%s(%d)]: "
	#define MRC_PROMPT_ARGS		__FILE__, __LINE__,
	#define MRC_ASSERT_PROMPT(x)	"Assertion \"" #x "\" failed:\n\t"
#else
	#define MRC_DEBUG_ONLY(x)
	#define MRC_PROMPT
	#define MRC_PROMPT_ARGS
	#define MRC_ASSERT_PROMPT(x)
#endif

//LOGGER

#define MRC_END_SEQ "\033[0m\n"
#define MRC_END fprintf(MRC_OUTPUT, MRC_END_SEQ)
#define MRC_BASE(color,x,...) fprintf(MRC_OUTPUT, "\033[1m\033[38;5;" #color "m" MRC_PROMPT x "%s", MRC_PROMPT_ARGS __VA_ARGS__)

#define MRC_BLOCK_TRACE(x)	MRC_BASE(8, "", ""); x; MRC_END;
#define MRC_BLOCK_DEBUG(x)	MRC_BASE(14, "", ""); x; MRC_END;
#define MRC_BLOCK_INFO(x)	MRC_BASE(2, "", ""); x; MRC_END;
#define MRC_BLOCK_WARN(x)	MRC_BASE(11, "", ""); x; MRC_END;
#define MRC_BLOCK_ERROR(x)	MRC_BASE(1, "", ""); x; MRC_END;
#define MRC_BLOCK_CRITIC(x)	MRC_BASE(88, "", ""); x; MRC_END;

#define MRC_PRINT(...)		fprintf(MRC_OUTPUT, __VA_ARGS__)
#define MRC_TRACE(...)		MRC_BASE(8, __VA_ARGS__, MRC_END_SEQ)
#define MRC_DEBUG(...)		MRC_BASE(14, __VA_ARGS__, MRC_END_SEQ)
#define MRC_INFO(...)		MRC_BASE(2, __VA_ARGS__, MRC_END_SEQ)
#define MRC_WARN(...)		MRC_BASE(11, __VA_ARGS__, MRC_END_SEQ)
#define MRC_ERROR(...)		MRC_BASE(1, __VA_ARGS__, MRC_END_SEQ)
#define MRC_CRITIC(...)		MRC_BASE(88, __VA_ARGS__, MRC_END_SEQ)

#define MRC_ASSERT(x,...) 	STATEMENT( if (!(x)) { MRC_CRITIC(MRC_ASSERT_PROMPT((x)) __VA_ARGS__); exit(-1); } )
#define MRC_ASSERT_NOT(x,...) 	STATEMENT( if (x) { MRC_CRITIC(MRC_ASSERT_PROMPT(!(x)) __VA_ARGS__); exit(-1); } )
#define MRC_ABORT(...)		STATEMENT( MRC_CRITIC(__VA_ARGS__); exit(-1); )

//DEBUG ONLY

#define _MRC_BLOCK_TRACE(x)	MRC_DEBUG_ONLY(MRC_BLOCK_TRACE(x))
#define _MRC_BLOCK_DEBUG(x)	MRC_DEBUG_ONLY(MRC_BLOCK_DEBUG(x))
#define _MRC_BLOCK_INFO(x)	MRC_DEBUG_ONLY(MRC_BLOCK_INFO(x))
#define _MRC_BLOCK_WARN(x)	MRC_DEBUG_ONLY(MRC_BLOCK_WARN(x))
#define _MRC_BLOCK_ERROR(x)	MRC_DEBUG_ONLY(MRC_BLOCK_ERROR(x))
#define _MRC_BLOCK_CRITIC(x)	MRC_DEBUG_ONLY(MRC_BLOCK_CRITIC(x))

#define _MRC_PRINT(...)		MRC_DEBUG_ONLY(MRC_PRINT(__VA_ARGS__))
#define _MRC_TRACE(...)		MRC_DEBUG_ONLY(MRC_TRACE(__VA_ARGS__))
#define _MRC_DEBUG(...)		MRC_DEBUG_ONLY(MRC_DEBUG(__VA_ARGS__))
#define _MRC_INFO(...)		MRC_DEBUG_ONLY(MRC_INFO(__VA_ARGS__))
#define _MRC_WARN(...)		MRC_DEBUG_ONLY(MRC_WARN(__VA_ARGS__))
#define _MRC_ERROR(...)		MRC_DEBUG_ONLY(MRC_ERROR(__VA_ARGS__))
#define _MRC_CRITIC(...)	MRC_DEBUG_ONLY(MRC_CRITIC(__VA_ARGS__))

#define _MRC_ASSERT(...)	MRC_DEBUG_ONLY(MRC_ASSERT(__VA_ARGS__))
#define _MRC_ASSERT_NOT(...)	MRC_DEBUG_ONLY(MRC_ASSERT_NOT(__VA_ARGS__))

#endif
