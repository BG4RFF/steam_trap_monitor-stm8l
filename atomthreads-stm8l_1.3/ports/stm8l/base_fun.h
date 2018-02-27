      
/**@file 
 * @note tiansixinxi. All Right Reserved.
 * @brief  
 * 
 * @author   madongfang
 * @date     2016-5-26
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

#ifndef _BASE_FUN_H_
#define _BASE_FUN_H_

#include "common.h"

/* ���Դ�ӡ */
#define DEBUG_NONE 0 // ��������Դ�ӡ��Ϣ
#define DEBUG_ERROR 1 // ���������Ϣ
#define DEBUG_WARN 2 // ���������Ϣ
#define DEBUG_NOTICE 3 // ���һ���Ƶ������Ϣ
#define DEBUG_INFO 4 // ���һ���Ƶ������Ϣ

/* ���ݲ�ͬ�ȼ�������Դ�ӡ��Ϣ */
#define DEBUG_PRINT(level, fmt, args...) \
	do\
	{\
		if (level <= get_debug_level())\
		{\
			if (DEBUG_ERROR == level)\
			{\
				printf("[%s][%s][%d]"fmt, "ERROR", __FILE__, __LINE__, ##args);\
			}\
			else if (DEBUG_WARN == level)\
			{\
				printf("[%s][%s][%d]"fmt, "WARNING", __FILE__, __LINE__, ##args);\
			}\
			else if (DEBUG_NOTICE == level)\
			{\
				printf("[%s][%s][%d]"fmt, "NOTICE", __FILE__, __LINE__, ##args);\
			}\
			else if (DEBUG_INFO == level)\
			{\
				printf("[%s][%s][%d]"fmt, "INFO", __FILE__, __LINE__, ##args);\
			}\
		}\
	}while(0)

/* ��ȫ�ͷŶ�̬������ڴ� */
#define SAFE_FREE(ptr) \
	do\
	{\
		if (NULL != ptr)\
		{\
			free(ptr);\
			ptr = NULL;\
		}\
	}while(0)

/* ��ȫ�ر������� */
#define SAFE_CLOSE(fd) \
	do\
	{\
		if (-1 != fd)\
		{\
			close(fd);\
			fd = -1;\
		}\
	}while(0)

#ifndef MAX
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#endif

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

extern INT32 get_debug_level(void);
extern void set_debug_level(INT32 level);
extern UINT32 get_build_date(INT8 *p_date_buff, INT32 buff_len);

#endif

