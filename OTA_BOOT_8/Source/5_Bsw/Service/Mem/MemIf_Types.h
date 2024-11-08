#if !defined MEMIF_TYPES_H_INCLUDED
#define MEMIF_TYPES_H_INCLUDED


typedef enum
{
    MEMIF_UNINIT,
    MEMIF_IDLE,
    MEMIF_BUSY,
    MEMIF_BUSY_INTERNAL
} MemIf_StatusType;


typedef enum
{
    MEMIF_JOB_OK,
    MEMIF_JOB_FAILED,
    MEMIF_JOB_PENDING,
    MEMIF_JOB_CANCELED,
    MEMIF_BLOCK_INCONSISTENT,
    MEMIF_BLOCK_INVALID
} MemIf_JobResultType;



typedef enum
{
    MEMIF_MODE_SLOW,
    MEMIF_MODE_FAST
} MemIf_ModeType;


#endif /* MEMIF_TYPES_H_INCLUDED */

