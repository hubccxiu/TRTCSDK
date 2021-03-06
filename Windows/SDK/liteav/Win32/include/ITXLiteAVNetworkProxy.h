#pragma once
#include "TXLiteAVBase.h"
class ITXNetworkProxy;

extern "C" {
    /**
    * \brief 用于动态加载dll时，导出TXNetworkProxy C++对象。
    *
    * \return TXNetworkProxy对象指针，注意：delete ITXNetworkProxy*会编译错误，需要调用destroyTXNetworkProxy释放。
    */
    LITEAV_API ITXNetworkProxy* createTXNetworkProxy();

    /**
    * \brief 析构ITXNetworkProxy对象
    */
    LITEAV_API void destroyTXNetworkProxy(ITXNetworkProxy** pTXNetworkProxy);
}

class ITXNetworkProxy
{
protected:
    virtual ~ITXNetworkProxy() {};
    
public:
    /**
    * \brief：设置透明代理，SDK通过此透明代理server转发到腾讯云
    * \param：ip  - 透明代理IP 
    * \return 无
    */
    virtual void setProxy(const char * ip, unsigned short port) = 0;

	/**
	* \brief：单独设置数据上报透明代理，setProxy 包含此接口的调用，
	* \param：ip  - 透明代理IP
	* \return 无
	*/
    virtual void setReportProxy(const char * ip, unsigned short port) = 0;
};
