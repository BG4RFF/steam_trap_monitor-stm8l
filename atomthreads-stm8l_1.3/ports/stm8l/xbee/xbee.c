      
/**@file 
 * @note 
 * @brief  
 * 
 * @author   madongfang
 * @date     2016-10-23
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

#include <atom.h>
#include <atomtimer.h>
#include "base_fun.h"
#include "uart.h"

extern INT8 g_unique_id[24];

static INT16 xbee_send_at(const INT8 *p_cmd)
{
	INT16 i = 0;

	if (NULL == p_cmd)
	{
		DEBUG_PRINT(DEBUG_ERROR, "param error\n");
		return -1;
	}

	gprs_serial_send_onebyte((UINT8)'A');
	gprs_serial_send_onebyte((UINT8)'T');
	for (i = 0; p_cmd[i] != '\0'; i++)
	{
		gprs_serial_send_onebyte((UINT8)p_cmd[i]);
	}
	gprs_serial_send_onebyte((UINT8)'\r');
	
	return 0;
}

/**		  
 * @brief		 ����xbee uart���ص����ݣ�һֱ��������ֱ���յ�\r���߳�ʱ���߳���
 * @param[out] p_buff �������ݻ�����
 * @param[in]  len �������ݻ�������С
 * @param[in]  timeout ��ʱʱ�� (0 = forever)
 * @return	 �ɹ����ػ�ȡ�����ַ����ĳ���(������\0)��ʧ�ܷ��ظ�����-2��ʾ��ʱ
 */
INT16 xbee_recv_ret_str(INT8 *p_buff, INT16 len, INT32 timeout)
{
	INT16 i = 0;
	INT8 ch = 0;
	INT16 ret = 0;

	if (NULL == p_buff || len < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "param error\n");
		return -1;
	}

	while ((ret = gprs_serial_read_onebyte((UINT8 *)&ch, timeout)) == 0)
	{
		if ('\r' == ch)
		{
			break;
		}
		else
		{
			if (i < len - 1)
			{
				p_buff[i] = ch;
				i++;
			}
			else
			{
				DEBUG_PRINT(DEBUG_WARN, "xbee_recv_ret_str exceed buff\n");
			}
		}
	}

	if (0 == ret)
	{
		p_buff[i] = '\0';
		return i;
	}
	else
	{
		
		DEBUG_PRINT(DEBUG_ERROR, "gprs_serial_read_onebyte error\n");
		return ret;
	}
}

static INT16 enter_command_mode(void)
{
	INT8 str[4] = {0};
	
	atomTimerDelay(SYSTEM_TICKS_PER_SEC);
	clear_gprs_buffer(); // �崮�ڻ���
	gprs_serial_send_onebyte((UINT8)'+');
	gprs_serial_send_onebyte((UINT8)'+');
	gprs_serial_send_onebyte((UINT8)'+');
	atomTimerDelay(SYSTEM_TICKS_PER_SEC);

	if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "send +++ but no response\n");
		return -1;
	}

	if (strcmp(str, "OK") != 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "send +++ but not recv OK\n");
		return -1;
	}
	return 0;
}

static INT16 exit_command_mode(void)
{
	INT8 str[4] = {0};
	
	xbee_send_at("CN");
	if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
		return -1;
	}
	
	if (strcmp(str, "OK") != 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "exit_command_mode not recv OK\n");
		return -1;
	}
	return 0;
}

void xbee_sleep(void)
{
	GPIO_SetBits(GPIOB, GPIO_Pin_5);
}

void xbee_wakeup(void)
{
	GPIO_ResetBits(GPIOB, GPIO_Pin_5);
}

INT16 xbee_init(void)
{
	INT8 str[32] = {0};
	INT16 len = 0;
	
	if (enter_command_mode() != 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee enter_command_mode failed\n");
		return -1;
	}

	strcpy(g_unique_id, "0000000000000000");
	
	xbee_send_at("SH");
	if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
		return -1;
	}
	len = strlen(str);
	strcpy(g_unique_id + 8 - len, str);

	xbee_send_at("SL");
	if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
		return -1;
	}
	len = strlen(str);
	strcpy(g_unique_id + 16 - len, str);
	printf("unique id:%s\n", g_unique_id);

	/* ����Ϊ��xbeeЭ���� */
	xbee_send_at("CE0");
	if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
		return -1;
	}
	if (strcmp(str, "OK") != 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "CE0 not recv OK\n");
		return -1;
	}

	/* Network Reset����ֹ�ڵ�����һ��������޷��ټ�����һ������ */
	xbee_send_at("NR0");
	if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
		return -1;
	}
	if (strcmp(str, "OK") != 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "NR0 not recv OK\n");
		return -1;
	}

	xbee_wakeup(); // ����SM1����ʱ�Ȳ�����
	/* ����ΪxbeeΪend device����Pin sleepģʽ */
	xbee_send_at("SM1");
	if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
		return -1;
	}
	if (strcmp(str, "OK") != 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "SM1 not recv OK\n");
		return -1;
	}

	/* ��ѯ�ȴ�ֱ���ڵ���뵽ĳ��zigbee���� */
	FOREVER
	{
		xbee_send_at("OP");
		if (xbee_recv_ret_str(str, sizeof(str), SYSTEM_TICKS_PER_SEC * 4) < 0)
		{
			DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
			return -1;
		}
		if (strcmp(str, "0") != 0)
		{
			DEBUG_PRINT(DEBUG_NOTICE, "join network, extended PAN ID = %s\n", str);
			break;
		}
		else
		{
			DEBUG_PRINT(DEBUG_INFO, "waiting for join network...\n");
		}
		atomTimerDelay(SYSTEM_TICKS_PER_SEC * 2);
	}

	exit_command_mode();

	return 0;
}

INT16 xbee_write(const void *p_data, INT16 len)
{
	UINT8 *ptr = (UINT8 *)p_data;
	INT16 i = 0;
	
	if (NULL == p_data)
	{
		DEBUG_PRINT(DEBUG_ERROR, "param error\n");
		return -1;
	}
	
	for (i = 0; i < len; i++)
	{
		gprs_serial_send_onebyte(ptr[i]);
	}

	return len;
}

INT16 xbee_get_time(INT8 *p_time, INT16 len)
{
	xbee_write("time", strlen("time"));

	if (xbee_recv_ret_str(p_time, len, SYSTEM_TICKS_PER_SEC * 4) < 0)
	{
		DEBUG_PRINT(DEBUG_ERROR, "xbee_recv_ret_str failed\n");
		return -1;
	}

	return 0;
}

