/**
 * @file hw_key.h
 * @brief Ӳ������������غ����ͽṹ������
 */

#ifndef _HW_KEY_H_
#define _HW_KEY_H_

#include "ti_msp_dl_config.h"

/**
 * @brief ����״̬�ṹ��
 *
 * �ýṹ��ʹ��λ�ֶ�����ʾÿ��������״̬���Խ�ʡ�ռ䡣
 */
typedef struct {
    unsigned int up : 1;    //!< �ϰ���״̬
    unsigned int left : 1;  //!< �󰴼�״̬
    unsigned int right : 1; //!< �Ұ���״̬
    unsigned int down : 1;  //!< �°���״̬
    unsigned int mid : 1;   //!< �м䰴��״̬
} KEY_STATUS;

/**
 * @brief ɨ�谴��״̬
 *
 * @return ��ǰ����״̬
 *
 * �ú�������ɨ�貢���ص�ǰ������״̬��
 */
KEY_STATUS key_scan(void);

#endif // _HW_KEY_H_
