      
/**@file 
 * @note tiansixinxi. All Right Reserved.
 * @brief  
 * 
 * @author  madongfang
 * @date     2016-5-26
 * @version  V1.0.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note  
 * @warning  
 */

#include "base_fun.h"

static INT32 debug_level = DEBUG_ERROR; // ���Դ�ӡ��Ϣ����ȼ�

/**		  
 * @brief ��ȡ���Դ�ӡ��Ϣ����ȼ�
 * @param ��
 * @return ���Դ�ӡ��Ϣ����ȼ�
 */
INT32 get_debug_level(void)
{
	return debug_level;
}

/**		  
 * @brief		���õ��Դ�ӡ��Ϣ����ȼ� 
 * @param[in] level ���Դ�ӡ��Ϣ����ȼ�
 * @return ��
 */
void set_debug_level(INT32 level)
{
	debug_level = level;
}

/**
 * @brief ��ȡ����������ڣ�����һ��������ʾ������,��p_date_buff����NULL����p_date_buff�з��ر������ڵ��ַ���
 * @param[out] p_date_buff �����ڰ��ո�ʽ"build yyyymmdd"���
 * @param[in] buff_len ��ű��������ַ����Ļ���������
 * @return ����һ��������ʾ����,16~31λ��ʾ���,8~15λ��ʾ�·�,0~7λ��ʾ����
 */
UINT32 get_build_date(INT8 *p_date_buff, INT32 buff_len)
{
	INT32 year = 0;
	INT32 month = 0;
	INT32 day = 0;
	INT8 month_name[4] = {0};
	const INT8 *all_mon_names[] 
		= {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	sscanf(__DATE__, "%s%ld%ld", month_name, &day, &year);

	for (month = 0; month < 12; month++)
	{
		if (strcmp(month_name, all_mon_names[month]) == 0)
		{
			break;
		}
	}
	month++;

	if (p_date_buff != NULL)
	{
		snprintf(p_date_buff, buff_len, "build %ld%02ld%02ld", year, month, day);
	}

	return (UINT32)((year << 16) | (month << 8) | day);
}

