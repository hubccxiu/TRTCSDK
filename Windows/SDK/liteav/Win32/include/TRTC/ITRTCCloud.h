#pragma once
/*
 * Module:   ITRTCCloud @ TXLiteAVSDK
 *
 * SDK VERSION "6.2.0.7007"
 *
 * Function: 腾讯云视频通话功能的主要接口类
 *
 * Nouns[1]: 主流 - TRTC 里称摄像头这一路的画面叫做“主流”（或主路）画面。
 * Nouns[2]: 辅流(substream) - TRTC 里称屏幕分享或者播片这一路的画面叫做“辅流”（或辅路）画面。
 * Nouns[3]: 播片(vodplay) - TRTC 的windows版本支持将本地的一个视频文件分享出去，这个功能成为“播片”。
 *
 * 创建/使用/销毁ITRTCCloud对象的示例代码：
 * ITRTCCloud *trtcCloud = getTRTCShareInstance();
 * if(trtcCloud)
 * {
 *     std::string version(trtcCloud->getSDKVersion());
 * }
 * 程序退出或者后续程序流程不需要使用ITRTCCloud对象时，释放单例对象ITRTCCloud
 * destroyTRTCShareInstance();
 * trtcCloud = NULL;
 */

#include "TRTCCloudCallback.h"
#include "TRTCCloudDef.h"
#include <windows.h>

class ITRTCCloud;

extern "C" {
    /// @name 创建与销毁ITRTCCloud单例
    /// @{
    /**
    * \brief 用于动态加载dll时，获取ITRTCCloud对象指针。
    *
    * \return 返回ITRTCCloud单例对象的指针，注意：delete ITRTCCloud*会编译错误，需要调用destroyTRTCCloud释放单例指针对象。
    */
    LITEAV_API ITRTCCloud* getTRTCShareInstance();

    /**
    * \brief 释放ITRTCCloud单例对象。
    */
    LITEAV_API void destroyTRTCShareInstance();
    /// @}
}

class ITRTCCloud
{
protected:
    virtual ~ITRTCCloud() {};

public:
    /////////////////////////////////////////////////////////////////////////////////
    //
    //                       设置 TRTCCloudCallback 回调
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 设置 TRTCCloudCallback 回调
    /// @{
    /**
    * \brief 添加事件回调
    * 
    * \param callback 事件回调指针
    */
    virtual void addCallback(ITRTCCloudCallback* callback) = 0;

    /**
    * \brief 移除事件回调
    *
    * \param callback 事件回调指针
    */
    virtual void removeCallback(ITRTCCloudCallback* callback) = 0;
    /// @}
    
    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （一）房间相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 房间相关接口函数
    /// @{
    /**
    * \brief 1.1 进入房间
    *
    * \param params 进房参数，详情参考TRTCParams定义
    * \param scene 应用场景，目前支持视频通话（VideoCall）和在线直播（Live）两种场景
    * \note 不管进房是否成功，都必须与exitRoom配对使用，在调用 exitRoom 前再次调用 enterRoom 函数会导致不可预期的错误问题
    */
    virtual void enterRoom(const TRTCParams& params, TRTCAppScene scene) = 0;

    /**
    * \brief 1.2 退出房间
    */
    virtual void exitRoom() = 0;

    /**
    * \brief 1.3 开启跨房连麦
    * \param params json形式的参数
    * {"roomId":910,"userId":"userA","sign":"sign string ..."}
    */
    virtual void connectOtherRoom(const char* params) = 0;

    /**
    * \brief 1.4 关闭跨房连麦
    */
    virtual void disconnectOtherRoom() = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （二）视频相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name  视频相关接口函数
    /// @{
    /**
    * \brief 2.1 启动本地摄像头采集和预览
    *
    * \param rendHwnd 承载预览画面的 HWND
    * \attention 这个接口会启动默认的摄像头，可以通过 setCurrentCameraDevice 接口选用其他摄像头
    */
    virtual void startLocalPreview(HWND rendHwnd) = 0;

    /**
    * \brief 2.2 关闭本地摄像头采集和预览
    */
    virtual void stopLocalPreview() = 0;

    /**
    * \brief 2.3 启动渲染远端用户视频画面
    *
    * \param userId   远端用户标识
    * \param rendHwnd 承载预览画面的窗口句柄
    * \note 在 onUserVideoAvailable 回调时，调用这个接口
    */
    virtual void startRemoteView(const char* userId, HWND rendHwnd) = 0;

    /**
    * \brief 2.4 停止渲染远端用户画面
    *
    * \param userId 远端用户标识
    */
    virtual void stopRemoteView(const char* userId) = 0;

    /**
    * \brief 2.5 停止渲染所有远端用户画面，如果有屏幕分享，则屏幕分享的画面也会一并被关闭
    */
    virtual void stopAllRemoteView() = 0;
	
	/**
    * \brief 2.6 是否屏蔽本地视频
    *
    * 当屏蔽或重新开启本地视频后，房间里的其它成员会收到 onUserVideoAvailable 回调通知。
    * \param mute true: 关闭视频上行，false: 开启视频上行
    */
    virtual void muteLocalVideo(bool mute) = 0;

    /**
    * \brief 2.7 设置视频编码器相关参数，该设置决定了远端用户看到的画面质量（同时也是云端录制出的视频文件的画面质量）
    *
    * \param params        视频编码参数，详情请参考 TRTCCloudDef.h 中 TRTCVideoEncParam 的定义
    */
    virtual void setVideoEncoderParam(const TRTCVideoEncParam& params) = 0;

	/**
    * \brief 2.8 设置网络流控相关参数，该设置决定了SDK在各种网络环境下的调控策略（比如弱网下是“保清晰”还是“保流畅”）
    *
    * \param params        网络流控参数，详情请参考 TRTCCloudDef.h 中 TRTCNetworkQosParam 的定义
    */
    virtual void setNetworkQosParam(const TRTCNetworkQosParam& params) = 0;
	
    /**
    * \brief 2.9 设置本地图像的渲染模式
    *
    * \param mode 填充（画面可能会被拉伸裁剪）还是适应（画面可能会有黑边）
    */
    virtual void setLocalViewFillMode(TRTCVideoFillMode mode) = 0;

    /**
    * \brief 2.10 设置远端图像的渲染模式
    *
    * \param userId 远端用户标识
    * \param mode 填充（画面可能会被拉伸裁剪）还是适应（画面可能会有黑边）
    */
    virtual void setRemoteViewFillMode(const char* userId, TRTCVideoFillMode mode) = 0;

    /**
    * \brief 2.11 设置本地图像的顺时针旋转角度
    *
    * \param rotation 支持 90、180、270 旋转角度
    */
    virtual void setLocalViewRotation(TRTCVideoRotation rotation) = 0;

    /**
    * \brief 2.12 设置远端图像的顺时针旋转角度
    *
    * \param userId 远端用户标识
    * \param rotation 支持 90、180、270 旋转角度
    */
    virtual void setRemoteViewRotation(const char* userId, TRTCVideoRotation rotation) = 0;

    /**
    * \brief 2.13 设置视频编码输出的（也就是远端用户观看到的，以及服务器录制下来的）画面方向
	*
    * \param rotation 支持 90、180、270 旋转角度
    */
    virtual void setVideoEncoderRotation(TRTCVideoRotation rotation) = 0;

    /**
    * \brief 2.14 开启大小画面双路编码模式
    *
    *  如果当前用户是房间中的主要角色（比如主播、老师、主持人...），并且使用 PC 或者 Mac 环境，可以开启该模式
    *  开启该模式后，当前用户会同时输出【高清】和【低清】两路视频流（但只有一路音频流）
    *  对于开启该模式的当前用户，会占用更多的网络带宽，并且会更加消耗 CPU 计算资源
    *  对于同一房间的远程观众而言，
    *  如果有些人的下行网络很好，可以选择观看【高清】画面
    *  如果有些人的下行网络不好，可以选择观看【低清】画面
    * \param enable 是否开启小画面编码
    * \param smallVideoParam 小流的视频参数，必须和 setLocalVideoQuality 接口的params参数具有相同的宽高比，也就是分辨率宽度和高度比值相同
    */
    virtual void enableSmallVideoStream(bool enable, const TRTCVideoEncParam& smallVideoParam) = 0;

    /**
    * \brief 2.15 选择某一路的画面质量：当网络不好时可以切换到低清晰度的小画面
    *
    *      如果对方没有开启双路视频模式，则此操作无效
    * \param userId 远端用户标识
    * \param type 大小流类型
    */
    virtual void setRemoteVideoStreamType(const char* userId, TRTCVideoStreamType type) = 0;

    /**
    * \brief 2.16 设定观看方优先选择的视频质量
    *
    *  低端设备推荐优先选择低清晰度的小画面
    *  如果对方没有开启双路视频模式，则此操作无效
    * \param type 默认大小流类型
    * \attention 默认优先选择大画面
    */
    virtual void setPriorRemoteVideoStreamType(TRTCVideoStreamType type) = 0;

    /**
    * \brief 2.18 设置摄像头本地预览是否开镜像
    *
    * \param mirror 是否开启预览镜像
    */
    virtual void setLocalVideoMirror(bool mirror) = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （三）音频相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 音频相关接口函数
    /// @{
		
	/**
    * \brief 3.1 开启本地音频流，该函数会启动麦克风采集，并将音频数据广播给房间里的其他用户
    *
    * \note TRTC SDK 并不会默认打开本地的麦克风采集。
    */
    virtual void startLocalAudio() = 0;

    /**
    * \brief 3.2 关闭本地音频流
    */
    virtual void stopLocalAudio() = 0;
	
    /**
    * \brief 3.3 是否屏蔽本地音频
    *
    * \note 屏蔽或重新开启本地音频后，房间里的其它成员会收到 onUserAudioAvailable 回调通知。
    * \param mute true: 关闭音频上行 false: 开启音频上行
    */
    virtual void muteLocalAudio(bool mute) = 0;

    /**
    * \brief 3.4 屏蔽指定远端音频
    *
    * \param userId 用户id
    * \param mute 开关
    */
    virtual void muteRemoteAudio(const char* userId, bool mute) = 0;

    /**
    * \brief 3.5 远端所有用户全部静音
    *
    * \param mute 开关
    */
    virtual void muteAllRemoteAudio(bool mute) = 0;

    /**
    * \brief 3.6 启用或关闭音量大小提示
    *
    *  开启后会在 onUserVoiceVolume 中获取到 SDK 对音量大小值的评估
    * \param interval 建议设置为大于 200 毫秒，最小不小于 20 毫秒, 设置为 0 表示关闭
    * \param smoothLevel  灵敏度，[0,10], 数字越大，波动越灵敏
    */
    virtual void enableAudioVolumeEvaluation(uint32_t interval, uint32_t smoothLevel) = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （四）摄像头相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 摄像头相关接口函数
    /// @{
    /**
    * \brief 4.1 查询摄像头列表
    *
    * \return 摄像头管理器对象指针 ITRTCDeviceCollection*
    *  示例代码：
    *  ITRTCDeviceCollection * pDevice = m_pCloud->getCameraDevicesList();
    *  for (int i = 0; i < pDevice->getCount(); i++)
    *  {
    *      std::wstring name = UTF82Wide(pDevice->getDeviceName(i));
    *  }
    *  pDevice->release();
    *  pDevice = null;
    * \note 如果delete ITRTCDeviceCollection*指针会编译错误，SDK维护ITRTCDeviceCollection对象的生命周期。
    */
    virtual ITRTCDeviceCollection* getCameraDevicesList() = 0;

    /**
    * \brief 4.2 设置要使用的摄像头
    *
    * \param deviceId 摄像头ID，getCameraDevicesList 接口获取得到
    */
    virtual void setCurrentCameraDevice(const char* deviceId) = 0;

    /**
    * \brief 4.2 获取当前使用的摄像头
    *
    * \return 摄像头名称，字符编码格式是UTF-8
    */
    virtual const char* getCurrentCameraDevice() = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （五）音频设备相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 音频设备相关接口函数
    /// @{
    /**
    * \brief 5.1 查询麦克风列表
    *
    * \return 麦克风管理器对象指针 ITRTCDeviceCollection*
    *  示例代码：
    *  ITRTCDeviceCollection * pDevice = m_pCloud->getMicDevicesList();
    *  for (int i = 0; i < pDevice->getCount(); i++)
    *  {
    *      std::wstring name = UTF82Wide(pDevice->getDeviceName(i));
    *  }
    *  pDevice->release();
    *  pDevice = null;
    * \note 如果delete ITRTCDeviceCollection*指针会编译错误，SDK维护ITRTCDeviceCollection对象的生命周期。
    */
    virtual ITRTCDeviceCollection* getMicDevicesList() = 0;

    /**
    * \brief 5.2 选择指定的麦克风作为录音设备，不调用该接口时，默认选择索引为0的麦克风
    *
    * \param micId 麦克风Id，getMicDevicesList 接口查询获取
    */
    virtual void setCurrentMicDevice(const char* micId) = 0;

    /**
    * \brief 5.3 获取当前选择的麦克风
    *
    * \return 麦克风名称，字符编码格式是UTF-8
    */
    virtual const char* getCurrentMicDevice() = 0;

    /**
    * \brief 5.4 查询已选择麦克风的音量
    *
    * \return 音量值，范围是[0, 100]
    */
    virtual uint32_t getCurrentMicDeviceVolume() = 0;

    /**
    * \brief 5.5 设置已选择麦克风的音量
    *
    * \param volume 设置的音量大小，范围是[0, 100]
    */
    virtual void setCurrentMicDeviceVolume(uint32_t volume) = 0;

    /**
    * \brief 5.6 查询扬声器列表
    *
    * \return 扬声器管理器对象指针 ITRTCDeviceCollection*
    *  示例代码：
    *  ITRTCDeviceCollection * pDevice = m_pCloud->getSpeakerDevicesList();
    *  for (int i = 0; i < pDevice->getCount(); i++)
    *  {
    *      std::wstring name = UTF82Wide(pDevice->getDeviceName(i));
    *  }
    *  pDevice->release();
    *  pDevice = null;
    * \note 如果delete ITRTCDeviceCollection*指针会编译错误，SDK维护ITRTCDeviceCollection对象的生命周期。
    */
    virtual ITRTCDeviceCollection* getSpeakerDevicesList() = 0;

    /**
    * \brief 5.7 选择指定的扬声器作为音频播放的设备，不调用该接口时，默认选择索引为0的扬声器
    *
    * \param speakerId 麦克风Id，getSpeakerDevicesList 接口查询获取
    */
    virtual void setCurrentSpeakerDevice(const char* speakerId) = 0;

    /**
    * \brief 5.8 获取已选择的扬声器
    *
    * \return 扬声器名称，字符编码格式是UTF-8
    */
    virtual const char* getCurrentSpeakerDevice() = 0;

    /**
    * \brief 5.9 查询已选择扬声器的音量，注意查询的不是系统扬声器的音量大小。
    *
    * \return 音量值，范围是[0, 100]
    */
    virtual uint32_t getCurrentSpeakerVolume() = 0;

    /**
    * \brief 5.10 设置SDK播放的音量，注意设置的不是系统扬声器的音量大小
    *
    * \param volume 设置的音量大小，范围是[0, 100]
    */
    virtual void setCurrentSpeakerVolume(uint32_t volume) = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （六）图像前处理相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 美颜相关接口函数
    /// @{
    /**
    * \brief 6.1 设置美颜、美白、红润
    *
    * \param style     美颜风格
    * \param beauty    美颜级别，取值范围 0 - 9: 0表示关闭，值越大，效果越明显
    * \param white     美白级别，取值范围 0 - 9: 0表示关闭，值越大，效果越明显
    * \param ruddiness 红润级别，取值范围 0 - 9: 0表示关闭，值越大，效果越明显，该参数暂未生效
    */
    virtual void setBeautyStyle(TRTCBeautyStyle style, uint32_t beauty, uint32_t white, uint32_t ruddiness) = 0;

    /**
	* \brief 6.2 设置水印
    * \param streamType 要设置水印的流类型(TRTCVideoStreamTypeBig、TRTCVideoStreamTypeSub)
	* \param srcData    水印图片源数据（传 NULL 表示去掉水印）
    * \param srcType    水印图片源数据类型（传 NULL 时忽略该参数）
    * \param nWidth     水印图片像素宽度（源数据为文件路径时忽略该参数）
    * \param nHeight    水印图片像素高度（源数据为文件路径时忽略该参数）
	* \param xOffset    水印显示的左上角x轴偏移
	* \param yOffset    水印显示的左上角y轴偏移
	* \param fWidthRatio 水印显示的宽度占画面宽度比例（水印按该参数等比例缩放显示）
    * \note 大小流暂未支持
	*/
    virtual void setWaterMark(TRTCVideoStreamType streamType, const char* srcData, TRTCWaterMarkSrcType srcType, uint32_t nWidth, uint32_t nHeight, float xOffset, float yOffset, float fWidthRatio) = 0;
    
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （七）辅流相关接口函数（屏幕共享，播片等）
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 辅流相关接口函数
    /// @{
    /**
    * \brief 7.1 开始渲染远端用户辅流画面，对应于 startRemoteView() 用于观看远端的主路画面，该接口只能用于观看辅路（屏幕分享、远程播片）画面
    * \param userId  远端用户标识
    * \param rendHwnd - 承载预览画面的 HWND
    * \note 在 onUserSubStreamAvailable 回调时，调用这个接口
    */
    virtual void startRemoteSubStreamView(const char* userId, HWND rendHwnd) = 0;

    /**
    * \brief 7.2 停止渲染远端用户辅流画面
    * \param userId 远端用户标识
    */
    virtual void stopRemoteSubStreamView(const char* userId) = 0;

    /**
    * \brief 7.3 设置辅流画面的渲染模式对应于setRemoteViewFillMode() 于设置远端的主路画面，该接口用于设置远端的辅路（屏幕分享、远程播片）画面
    *
    * \param userId 用户的id
    * \param mode 填充（画面可能会被拉伸裁剪）还是适应（画面可能会有黑边）
    */
    virtual void setRemoteSubStreamViewFillMode(const char* userId, TRTCVideoFillMode mode) = 0;
	
	/**
    * \brief 7.4 【屏幕共享】枚举可共享的窗口列表，
	*
    * \param thumbSize - 指定要获取的窗口缩略图大小，缩略图可用于绘制在窗口选择界面上
    * \param iconSize  - 指定要获取的窗口图标大小
    *
    * \return 列表通过返回值 ITRTCScreenCaptureSourceList 返回
    * \note   如果delete ITRTCScreenCaptureSourceList*指针会编译错误，SDK维护ITRTCScreenCaptureSourceList对象的生命周期。
    */
    virtual ITRTCScreenCaptureSourceList* getScreenCaptureSources(const SIZE &thumbSize, const SIZE &iconSize) = 0;

    /**
    * \brief 7.5 【屏幕共享】选择要分享的目标窗口或目标区域，支持如下四种情况：
	*
	* >>> 共享整个屏幕 : sourceInfoList 中type为Screen的source，captureRect 设为 { 0, 0, 0, 0 }
    * >>> 共享指定区域 : sourceInfoList 中type为Screen的source，captureRect 设为非 NULL，比如 { 100, 100, 300, 300 }
    * >>> 共享整个窗口 : sourceInfoList 中type为Window的source，captureRect 设为 { 0, 0, 0, 0 }
    * >>> 共享窗口区域 : sourceInfoList 中type为Window的source，captureRect 设为非 NULL，比如 { 100, 100, 300, 300 }
	*
	* \note: 您可以在屏幕分享的过程中掉用该函数来切换目标窗口或者调整目标区域
	*
    * \param source             - 指定分享源
    * \param captureRect        - 指定捕获的区域
    * \param captureMouse       - 指定是否捕获鼠标指针
    * \param highlightWindow    - 指定是否高亮正在共享的窗口以及当捕获图像被遮挡时高亮遮挡窗口提示用户移走遮挡
    *
    */
    virtual void selectScreenCaptureTarget(const TRTCScreenCaptureSourceInfo &source, const RECT& captureRect, bool captureMouse = true, bool highlightWindow = true) = 0;

    /**
    * \brief 7.6 【屏幕共享】启动屏幕分享
    * \param：rendHwnd - 承载预览画面的 HWND
    */
    virtual void startScreenCapture(HWND rendHwnd) = 0;

    /**
    * \brief 7.7 【屏幕共享】暂停屏幕分享
    */
    virtual void pauseScreenCapture() = 0;

    /**
    * \brief 7.8 【屏幕共享】恢复屏幕分享
    */
    virtual void resumeScreenCapture() = 0;

    /**
    * \brief 7.* 【屏幕共享】关闭屏幕分享
    */
    virtual void stopScreenCapture() = 0;

	/**
    * \brief 7.10 设置辅路视频编码器参数，对应于 setVideoEncoderParam() 设置主路画面的编码质量
	*             该设置决定了远端用户看到的画面质量（同时也是云端录制出的视频文件的画面质量）
    *
    * \param params   辅流编码参数，详情请参考 TRTCCloudDef.h 中的 TRTCVideoEncParam 定义
    */
    virtual void setSubStreamEncoderParam(const TRTCVideoEncParam& params) = 0;

    /**
    * \brief 7.11 设置辅流的混音音量大小，这个数值越高，辅流音量占比就约高，麦克风音量占比就越小
    *
    * \param volume 设置的混音音量大小，范围是[0, 100]
    */
    virtual void setSubStreamMixVolume(uint32_t volume) = 0;
    
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （八）自定义采集和渲染
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 自定义采集和渲染
    /// @{

    /**
    * \brief 8.1 启用视频自定义采集模式，即放弃SDK原来的视频采集流程，改用sendCustomVideoData向SDK塞入自己采集的视频画面
    * \param enable 是否启用
    */
    virtual void enableCustomVideoCapture(bool enable) = 0;

    /**
    * \brief 8.2 发送自定义的SampleBuffer
    * \note SDK内部不做帧率控制,请务必保证调用该函数的频率和setVideoEncoderParam中设置的帧率一致,否则编码器输出的码率会不受控制
    * \param frame 视频数据，仅支持PixelBuffer I420数据
    */
    virtual void sendCustomVideoData(TRTCVideoFrame* frame) = 0;

	 /**
    * \brief 8.3 设置本地视频自定义渲染
    *
    * \note              设置此方法，SDK内部会把采集到的数据回调出来，SDK跳过HWND渲染逻辑
                         调用 setLocalVideoRenderCallback(TRTCVideoPixelFormat_Unknown, TRTCVideoBufferType_Unknown, nullptr) 停止回调
    * \param pixelFormat 指定回调的像素格式
    * \param bufferType  指定视频数据结构类型
    * \param callback    自定义渲染回调
    * \return 0：成功， <0：错误
    */
    virtual int setLocalVideoRenderCallback(TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback* callback) = 0;

    /**
    * \brief 8.4 设置远端视频自定义渲染
    *
    * \note              设置此方法，SDK内部会把远端的数据解码后回调出来，SDK跳过HWND渲染逻辑
                         调用 setRemoteVideoRenderCallback(userid,TRTCVideoPixelFormat_Unknown, TRTCVideoBufferType_Unknown, nullptr) 停止回调。
    * \param userId      用户标识
    * \param pixelFormat 指定回调的像素格式
    * \param bufferType  指定视频数据结构类型
    * \param callback    自定义渲染回调
    * \return 0：成功， <0：错误
    */
    virtual int setRemoteVideoRenderCallback(const char* userId, TRTCVideoPixelFormat pixelFormat, TRTCVideoBufferType bufferType, ITRTCVideoRenderCallback* callback) = 0;
	
    /**
    * \brief 8.5 设置音频数据的相关回调
    *
    * \note              设置此方法，SDK内部会把声音模块的数据（PCM格式）回调出来，包括：
    *                    1. 本地麦克风录制的到的声音
    *                    2. 每一路远程用户的声音
    *                    3. 混合后的要送往喇叭进行播放的声音
    *                    调用 setAudioFrameCallback(nullptr) 停止回调。
    *
    * \param callback          声音帧数据（PCM格式）的回调
    * \return 0：成功， <0：错误
    */
    virtual int setAudioFrameCallback(ITRTCAudioFrameCallback* callback) = 0;

    /**
    * \brief 8.6 调用实验性API接口
    *
    * \note 该接口用于调用一些实验性功能
    * \param jsonStr 接口及参数描述的json字符串
    */
    virtual void callExperimentalAPI(const char *jsonStr) = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （九）自定义消息发送
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 自定义消息发送
    /// @{
    /**
    * \brief 9.1 发送自定义消息给房间内所有用户
    *
    * \param cmdId    消息ID，取值范围为 1 ~ 10
    * \param data     待发送的数据，最大支持 1KB（1000字节）的数据大小
    * \param dataSize 待发送的数据大小
    * \param reliable 是否可靠发送，可靠发送的代价是会引入一定的延时，因为接收端要暂存一段时间的数据来等待重传
    * \param ordered  是否要求有序，即是否要求接收端接收的数据顺序和发送端发送的顺序一致，这会带来一定的接收延时，因为在接收端需要暂存并排序这些消息
    * \return true：消息已经发出 false：消息发送失败
	*
    * \note 限制1：发送消息到房间内所有用户，每秒最多能发送 30 条消息
    *       限制2：每个包最大为 1 KB，超过则很有可能会被中间路由器或者服务器丢弃
    *       限制3：每个客户端每秒最多能发送总计 8 KB 数据
    *
    *       请将 reliable 和 ordered 同时设置为 YES 或 NO, 暂不支持交叉设置。
    *       有序性（ordered）是指相同 cmdID 的消息流一定跟发送方的发送顺序相同，
    *       强烈建议不同类型的消息使用不同的 cmdID，这样可以在要求有序的情况下减小消息时延
    */
    virtual bool sendCustomCmdMsg(uint32_t cmdId, const uint8_t* data, uint32_t dataSize, bool reliable, bool ordered) = 0;
    
    /**
    * \brief 9.2 发送自定义消息给房间内所有用户
    *
    * \param data          待发送的数据，最大支持 1kb（1000字节）的数据大小
    * \param repeatCount   发送数据次数
    * \return YES:消息已通过限制，等待后续视频帧发送 NO:消息被限制发送
    *
    * \note 限制1：数据在接口调用完后不会被即时发送出去，而是从下一帧视频帧开始带在视频帧中发送
    *       限制2：发送消息到房间内所有用户，每秒最多能发送 30 条消息 (**与sendCustomCmdMsg共享限制**)
    *       限制2：每个包最大为1KB，若发送大量数据，会导致视频码率增大，可能导致视频画质下降甚至卡顿 (**与sendCustomCmdMsg共享限制**)
    *       限制4：每个客户端每秒最多能发送总计 8 KB 数据 (**与sendCustomCmdMsg共享限制**)
    *       限制5：若指定多次发送（repeatCount>1）,则数据会被带在后续的连续repeatCount个视频帧中发送出去，同样会导致视频码率增大
    *       限制6: 如果repeatCount>1,多次发送，接收消息onRecvSEIMsg回调也可能会收到多次相同的消息，需要去重
    */
    virtual bool sendSEIMsg(const uint8_t* data, uint32_t dataSize, int32_t repeatCount) = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十）背景混音相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 背景混音相关接口函数
    /// @{
    /**
    * \brief 10.1 播放背景音乐
    *
    * \param path 音乐文件路径
    */
    virtual void playBGM(const char* path) = 0;

    /**
    * \brief 10.2 停止播放背景音乐
    */
    virtual void stopBGM() = 0;

    /**
    * \brief 10.3 暂停播放背景音乐
    */
    virtual void pauseBGM() = 0;

    /**
    * \brief 10.4 继续播放背景音乐
    */
    virtual void resumeBGM() = 0;

    /**
    * \brief 10.5 获取音乐文件总时长，单位毫秒
    *
    * \param path 音乐文件路径，如果path为空，那么返回当前正在播放的music时长
    * \return     返回音频文件的时长，单位ms。
    */
    virtual uint32_t getBGMDuration(const char* path) = 0;

    /**
    * \brief 10.6 设置BGM播放进度
    *
    * \param pos 单位毫秒
    */
    virtual void setBGMPosition(uint32_t pos) = 0;

    /**
    * \brief 10.7 设置麦克风的音量大小，播放背景音乐混音时使用，用来控制麦克风音量大小
    *
    * \param volume 音量大小，100为正常音量，值为0~200
    */
    virtual void setMicVolumeOnMixing(uint32_t volume) = 0;

    /**
    * \brief 10.8 设置背景音乐的音量大小，播放背景音乐混音时使用，用来控制背景音音量大小
    *
    * \param volume 音量大小，100为正常音量，值为0~200
    */
    virtual void setBGMVolume(uint32_t volume) = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十一）设备和网络测试
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 设备和网络测试
    /// @{
    /**
    * \brief 11.1 开始进行网络测速(视频通话期间请勿测试，以免影响通话质量)
    *
    * 监听 ITRTCCloudCallback::onSpeedTest 回调获取测速结果
    * 测速结果将会用于优化 SDK 接下来的服务器选择策略，因此推荐您在用户首次通话前先进行一次测速，这将有助于我们选择最佳的服务器
    * 同时，如果测试结果非常不理想，您可以通过醒目的 UI 提示用户选择更好的网络
    * 
    * 注意：测速本身会消耗一定的流量，所以也会产生少量额外的流量费用
    *
    * \param sdkAppId 应用标识
    * \param userId 用户标识
    * \param userSig 用户签名
    */
    virtual void startSpeedTest(uint32_t sdkAppId, const char* userId, const char* userSig) = 0;

    /**
    * \brief 11.2 停止服务器测速
    */
    virtual void stopSpeedTest() = 0;

    /**
    * \brief 11.3 开启摄像头测试，会触发 onLocalVideoFrameAfterProcess 回调接口
    *
    * \param rendHwnd 承载预览画面的 HWND
    * \attention 这个接口会启动默认的摄像头，可以通过 setCurrentCameraDevice 接口指定其他摄像头
    */
    virtual void startCameraDeviceTest(HWND rendHwnd) = 0;

    /**
    * \brief 11.4 停止摄像头测试
    */
    virtual void stopCameraDeviceTest() = 0;

    /**
    * \brief 11.5 开启麦克风测试，回调接口 onTestMicVolume 获取测试数据
    *
    * \param interval 反馈音量提示的时间间隔（ms），建议设置到大于 200 毫秒
    */
    virtual void startMicDeviceTest(uint32_t interval) = 0;

    /**
    * \brief 11.6 关闭麦克风测试
    */
    virtual void stopMicDeviceTest() = 0;

    /**
    * \brief 11.7 开启扬声器测试，回调接口 onTestSpeakerVolume 获取测试数据
    *
    * 该方法测试扬声器是否能正常工作。SDK播放指定的音频文件，测试者如果能听到声音，说明播放设备能正常工作
    * \param testAudioFilePath 音频文件的绝对路径，路径字符串使用 UTF-8 编码格式，支持文件格式: wav、mp3
    */
    virtual void startSpeakerDeviceTest(const char* testAudioFilePath) = 0;

    /**
    * \brief 11.8 停止扬声器测试
    */
    virtual void stopSpeakerDeviceTest() = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十二）混流转码并发布到CDN
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name 混流转码并发布到CDN
    /// @{
    /**
    * 12.1 启动CDN发布：通过腾讯云将当前房间的音视频流发布到直播CDN上
    *
    * 由于 TRTC 的线路费用是按照时长收费的，并且房间容量有限（< 1000人）
    * 当您有大规模并发观看的需求时，将房间里的音视频流发布到低成本高并发的直播CDN上是一种较为理想的选择。
    *
    * 目前支持两种发布方案：
    *
    * 【1】需要您先调用 setMixTranscodingConfig 对多路画面进行混合，发布到CDN上的是混合之后的音视频流
    *
    * 【2】发布当前房间里的各路音视频画面，每一路画面都有一个独立的地址，相互之间无影响
    *
    * \param param 请参考 TRTCCloudDef.h 中关于 TRTCPublishCDNParam 的介绍
    */
    virtual void startPublishCDNStream(const TRTCPublishCDNParam& param) = 0;

    /**
    * 12.2 停止CDN发布
    */
    virtual void stopPublishCDNStream() = 0;

    /**
    * \brief 12.3 启动(更新)云端的混流转码：通过腾讯云的转码服务，将房间里的多路画面叠加到一路画面上
    * \desc
    * <pre>
    * 【画面1】=> 解码 => =>
    *                        \
    * 【画面2】=> 解码 =>  画面混合 => 编码 => 【混合后的画面】
    *                        /
    * 【画面3】=> 解码 => =>
    * </pre>
    * \param config 请参考 TRTCCloudDef.h 中关于 TRTCTranscodingConfig 的介绍
    *               传入NULL取消云端混流转码
    */
    virtual void setMixTranscodingConfig(TRTCTranscodingConfig* config) = 0;
    /// @}

    /////////////////////////////////////////////////////////////////////////////////
    //
    //                      （十三）LOG相关接口函数
    //
    /////////////////////////////////////////////////////////////////////////////////
    /// @name LOG相关接口函数
    /// @{
    /**
    * \brief  13.1 获取SDK版本信息
    *
    * \param UTF8编码的版本号。
    */
    virtual const char* getSDKVersion() = 0;

    /**
    * \brief 13.2 设置log输出级别
    *
    * \param level 参见 TRTCLogLevel
    */
    virtual void setLogLevel(TRTCLogLevel level) = 0;

    /**
    * \brief 13.3 启用或禁用控制台日志打印
    *
    * \param enabled 指定是否启用
    */
    virtual void setConsoleEnabled(bool enabled) = 0;

    /**
    * \brief 13.4 启用或禁用Log的本地压缩
    *
    *  开启压缩后，log存储体积明显减小，但需要腾讯云提供的 python 脚本解压后才能阅读
    *  禁用压缩后，log采用明文存储，可以直接用记事本打开阅读，但占用空间较大。
    * \param enabled 指定是否启用
    */
    virtual void setLogCompressEnabled(bool enabled) = 0;

    /**
    * \brief 13.5 设置日志保存路径
    *
    * \param path 存储日志的文件夹，例如 "D:\\Log"，utf-8编码
    * \attention 如果没有调用这个接口，SDK 会将日志输出到默认的位置，位于 C:/Users/[系统用户名]/AppData/Roaming/Tencent/liteav/log，即 %appdata%/Tencent/liteav/log
    */
    virtual void setLogDirPath(const char* path) = 0;

    /**
    * \brief 13.6 设置日志回调
    *
    * \param callback 日志回调
    */
    virtual void setLogCallback(ITRTCLogCallback* callback) = 0;

    /**
    * \brief 13.7 显示仪表盘（状态统计和事件消息浮层view），方便调试
    *
    * \param showType 0: 不显示 1: 显示精简版 2: 显示全量版
    */
    virtual void showDebugView(int showType) = 0;
    /// @}
};