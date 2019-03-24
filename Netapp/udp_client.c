/**
  *****************************************************************************
  * @file    udp_client.c
  * @author  Zorb
  * @version V1.0.0
  * @date    2018-09-04
  * @brief   udp�ͻ��˵�ʵ��
  *****************************************************************************
  * @history
  *
  * 1. Date:2018-09-04
  *    Author:Zorb
  *    Modification:�����ļ�
  *
  *****************************************************************************
  */

#include "stm32f4xx_hal.h"
#include "lwip.h"
#include "udp.h"
#include "string.h"
#include "udp_client.h"

unsigned char remote_host_master_addr[4] = {192, 168, 1, 7};
unsigned char remote_host_backup_addr[4] = {192, 168, 1, 1};
unsigned int remote_host_master_connected = 0;

/* udp���ƿ� */
static struct udp_pcb *disc_upcb;
static struct udp_pcb *modbus_upcb;

/******************************************************************************
 * ����  : ����udp�ͻ���
 * ����  : ��
 * ����  : ��
******************************************************************************/

static void udp_disc_receive_callback(void *arg, struct udp_pcb *upcb,
    struct pbuf *p, const ip_addr_t *addr, u16_t port)
{    
    /* ���ݻش� */
//    udp_send(upcb, p);
//    udp_sendto(upcb, p, addr, port);
    
    /* ��ӡ���յ������� */
    printf("get disc msg from %d:%d:%d:%d port:%d:\r\n",
        *((uint8_t *)&addr->addr), *((uint8_t *)&addr->addr + 1),
        *((uint8_t *)&addr->addr + 2), *((uint8_t *)&addr->addr + 3), port);

    // We will add code to report my state, configuration
    udp_sendto(disc_upcb, p, addr, port);    

    if (remote_host_master_connected == 1 
      && remote_host_master_addr[0] == *((uint8_t *)&addr->addr)
      && remote_host_master_addr[1] == *((uint8_t *)&addr->addr + 1)
      && remote_host_master_addr[2] == *((uint8_t *)&addr->addr + 2)
      && remote_host_master_addr[3] == *((uint8_t *)&addr->addr + 3)) {
      // Report the client state
      remote_host_master_connected = 1;
      printf("Send only\r\n");
      udp_modbus_client_send("This is a UWB client");
    } else {
      remote_host_master_addr[0] = *((uint8_t *)&addr->addr);
      remote_host_master_addr[1] = *((uint8_t *)&addr->addr + 1);
      remote_host_master_addr[2] = *((uint8_t *)&addr->addr + 2);
      remote_host_master_addr[3] = *((uint8_t *)&addr->addr + 3);
      if (ERR_OK == udp_modbus_client_init()) {
        // Report the client state
        remote_host_master_connected = 1;
        printf("Send and create udp socke\r\n");
        udp_modbus_client_send("This is a UWB client");
      } else {
        remote_host_master_connected = 0;
      }
    }

    // Now we have get the host ip addres, it's time to report my configuraiton 
    // and ability
    // We need check the state, if the server was not changed and the client 
    // Have connected to server not need connect agagin
    
    if (ERR_OK == udp_modbus_client_init()) {
      // Report the client state
      remote_host_master_connected = 1;
      udp_modbus_client_send("This is a UWB client");
    }
    
    /* �ͷŻ��������� */
    pbuf_free(p);
}

static void udp_modbus_receive_callback(void *arg, struct udp_pcb *upcb,
    struct pbuf *p, const ip_addr_t *addr, u16_t port)
{
    uint32_t i;
    
    /* ���ݻش� */
//    udp_send(upcb, p);
//    udp_sendto(upcb, p, addr, port);
    
    /* ��ӡ���յ������� */
    printf("get modbus msg from %d:%d:%d:%d port:%d:\r\n",
        *((uint8_t *)&addr->addr), *((uint8_t *)&addr->addr + 1),
        *((uint8_t *)&addr->addr + 2), *((uint8_t *)&addr->addr + 3), port);
    
    if (p != NULL)
    {
        struct pbuf *ptmp = p;
        
        while(ptmp != NULL)
        {
            for (i = 0; i < p->len; i++)
            {
                printf("%x", *((char *)p->payload + i));
            }
            
            ptmp = p->next;
        }
        
        printf("\r\n");
    }
    
    /* �ͷŻ��������� */
    pbuf_free(p);
}

/******************************************************************************
 * ����  : ����udp����
 * ����  : (in)pData �������ݵ�ָ��
 * ����  : ��
******************************************************************************/
void udp_disc_client_send(char *pData)
{
    struct pbuf *p;
    
    /* ���仺�����ռ� */
    p = pbuf_alloc(PBUF_TRANSPORT, strlen(pData), PBUF_POOL);
    
    if (p != NULL)
    {
        /* ��仺�������� */
        pbuf_take(p, pData, strlen(pData));

        /* ����udp���� */
        udp_send(disc_upcb, p);

        /* �ͷŻ������ռ� */
        pbuf_free(p);
    }
}

/******************************************************************************
 * ����  : ����udp����
 * ����  : (in)pData �������ݵ�ָ��, (in)len data to be send
 * ����  : ��
******************************************************************************/
void udp_disc_client_raw_send(char *pData, int len)
{
    struct pbuf *p;
    
    /* ���仺�����ռ� */
    p = pbuf_alloc(PBUF_TRANSPORT, len, PBUF_POOL);
    
    if (p != NULL)
    {
        /* ��仺�������� */
        pbuf_take(p, pData, len);

        /* ����udp���� */
        udp_send(disc_upcb, p);

        /* �ͷŻ������ռ� */
        pbuf_free(p);
    }
}

/******************************************************************************
 * ����  : ����udp�ͻ���
 * ����  : ��
 * ����  : ��
******************************************************************************/
void udp_disc_client_init(void)
{
    ip_addr_t discServerIP;
    err_t err;
    
    ip4_addr_set_any(&discServerIP);
    
    /* ����udp���ƿ� */
    disc_upcb = udp_new();

    if (disc_upcb!=NULL)
    {
        /* ���ñ��ض˿� */
        disc_upcb->local_port = UDP_DISC_PORT_LOCAL;
        
        /* ���÷�����IP�Ͷ˿� */
        err = udp_connect(disc_upcb, &discServerIP, UDP_DISC_PORT_REMOTE);

        if (err == ERR_OK)
        {
            /* ע����ջص����� */
            udp_recv(disc_upcb, udp_disc_receive_callback, NULL);
            
            /* ����udp���� */
            udp_disc_client_send("udp client connected");
            
            printf("udp client connected\r\n");
        }
        else
        {
            udp_remove(disc_upcb);
            
            printf("can not connect udp pcb\r\n");
        }
    }
}

/******************************************************************************
 * ����  : ����udp����
 * ����  : (in)pData �������ݵ�ָ��
 * ����  : ��
******************************************************************************/
void udp_modbus_client_send(char *pData)
{
    struct pbuf *p;
    
    /* ���仺�����ռ� */
    p = pbuf_alloc(PBUF_TRANSPORT, strlen(pData), PBUF_POOL);
    
    if (p != NULL)
    {
        /* ��仺�������� */
        pbuf_take(p, pData, strlen(pData));

        /* ����udp���� */
        udp_send(modbus_upcb, p);

        /* �ͷŻ������ռ� */
        pbuf_free(p);
    }
}

/******************************************************************************
 * ����  : ����udp�ͻ���
 * ����  : ��
 * ����  : ��
******************************************************************************/
/******************************************************************************
 * ����  : ����udp�ͻ���
 * ����  : ��
 * ����  : ��
******************************************************************************/
int udp_modbus_client_init(void)
{
    ip_addr_t modbusServerIP;
    err_t err;

    IP4_ADDR(&modbusServerIP, remote_host_master_addr[0],
                              remote_host_master_addr[1],
                              remote_host_master_addr[2],
                              remote_host_master_addr[3]);

    /* ����udp���ƿ� */
    modbus_upcb = udp_new();

    if (modbus_upcb!=NULL)
    {
        /* ���ñ��ض˿� */
        modbus_upcb->local_port = UDP_MODBUS_PORT_LOCAL;
        
        /* ���÷�����IP�Ͷ˿� */
        err= udp_connect(modbus_upcb, &modbusServerIP, UDP_MODBUS_PORT_REMOTE);

        if (err == ERR_OK)
        {
            /* ע����ջص����� */
            udp_recv(modbus_upcb, udp_modbus_receive_callback, NULL);
            
            /* ����udp���� */
            //udp_modbus_client_send("udp client connected");
            
            printf("udp client connected\r\n");
        }
        else
        {
            udp_remove(modbus_upcb);
            
            printf("can not connect udp pcb\r\n");
        }
    } else {
      err = -1;
    }

    return err;
}

/******************************** END OF FILE ********************************/
