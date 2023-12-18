#ifndef DCAIQ_API_H
#define DCAIQ_API_H

#include "rkaiq/uAPI/rk_aiq_user_api_sysctl.h"
#include "rkaiq/uAPI/rk_aiq_user_api_imgproc.h"

#ifdef  __cplusplus
#define DCAIQ_BEGIN_DECLARE  extern "C" {
#define DCAIQ_END_DECLARE    }
#else
#define DCAIQ_BEGIN_DECLARE
#define DCAIQ_END_DECLARE
#endif

typedef rk_aiq_sys_ctx_t dc_aiq_sys_ctx_t;
typedef rk_aiq_isp_stats_t dc_aiq_isp_stats_t;
typedef rk_aiq_error_cb dc_aiq_error_cb;
typedef rk_aiq_metas_cb dc_aiq_metas_cb;
typedef rk_aiq_static_info_t dc_aiq_static_info_t;
typedef rk_aiq_metas_t dc_aiq_metas_t;
typedef rk_aiq_cpsl_cap_t dc_aiq_cpsl_cap_t;
typedef rk_aiq_exposure_sensor_descriptor dc_aiq_exposure_sensor_descriptor;
typedef rk_aiq_cpsl_info_t dc_aiq_cpsl_info_t;
typedef rk_aiq_ver_info_t dc_aiq_ver_info_t;
typedef rk_aiq_raw_prop_t dc_aiq_raw_prop_t;
typedef rk_aiq_rect_t dc_aiq_rect_t;

typedef rk_aiq_lens_vcmcfg dc_aiq_lens_vcmcfg;
typedef rk_aiq_af_sec_path_t dc_aiq_af_sec_path_t;
typedef rk_aiq_af_result_t dc_aiq_af_result_t;
typedef rk_aiq_af_zoomrange dc_aiq_af_zoomrange;
typedef rk_aiq_af_focusrange dc_aiq_af_focusrange;
typedef rk_aiq_gamma_attrib_t dc_aiq_gamma_attrib_t;
typedef rk_aiq_gray_mode_t dc_aiq_gray_mode_t;
typedef rk_aiq_af_algo_meas_t dc_aiq_af_algo_meas_t;
typedef rk_aiq_wb_scene_t dc_aiq_wb_scene_t;
typedef rk_aiq_wb_gain_t dc_aiq_wb_gain_t;
typedef rk_aiq_wb_cct_t dc_aiq_wb_cct_t;


/*********************************************
 * System Control
 *********************************************/

/*!
 * \brief pre-settings before init
 *
 * \param[in] sns_ent_name    active sensor media entity name. This represents the unique camera module
 *                            in system. And the whole active pipeline could be retrieved by this.
 * \param[in] mode            pipleline working mode, just used by \ref
 *                            dc_aiq_uapi_sysctl_init to select iq file
 *                            according to hdr mode. Unused if params \ref
 *                            iq_file is specified.
 * \param[in] iq_file         optional, forcely used this iq file
 * \note Optional API, should be called before\ref dc_aiq_uapi_sysctl_init. This
 *       API extends the functionality of \ref dc_aiq_uapi_sysctl_init. And just
 *       used to help select the satisfied iq file when multiple iq files
 *       exsist.
 */
XCamReturn dc_aiq_uapi_sysctl_preInit(const char* sns_ent_name, dc_aiq_working_mode_t mode, const char* force_iq_file);

/*!
 * \brief initialze aiq control system context
 * Should call before any other APIs
 *
 * \param[in] sns_ent_name    active sensor media entity name. This represents the unique camera module\n
 *                            in system. And the whole active pipeline could be retrieved by this.
 * \param[in] iq_file_dir     define the search directory of the iq files.
 * \param[in] err_cb          not mandatory. it's used to return system errors to user.
 * \param[in] metas_cb        not mandatory. it's used to return the metas(sensor exposure settings,\n
 *                            isp settings, etc.) for each frame
 * \return return system context if success, or NULL if failure.
 */
dc_aiq_sys_ctx_t* dc_aiq_uapi_sysctl_init(const char* sns_ent_name, const char* iq_file_dir, 
                                          dc_aiq_error_cb err_cb, dc_aiq_metas_cb metas_cb);

/*!
 * \brief deinitialze aiq context
 * Should not be called in started state
 *
 * \param[in] ctx             the context returned by \ref dc_aiq_uapi_sysctl_init
 */
void dc_aiq_uapi_sysctl_deinit( dc_aiq_sys_ctx_t* ctx);

/*!
 * \brief prepare aiq control system before runninig
 * prepare AIQ running enviroment, should be called before \ref dc_aiq_uapi_sysctl_start.\n
 * And if re-prepared is required after \ref dc_aiq_uapi_sysctl_start is called,\n
 * should call \ref dc_aiq_uapi_sysctl_stop firstly.
 *
 * \param[in] ctx             the context returned by \ref dc_aiq_uapi_sysctl_init
 * \param[in] width           sensor active output width, just used to check internally
 * \param[in] height          sensor active output height, just used to check internally
 * \param[in] mode            pipleline working mode
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_prepare(const dc_aiq_sys_ctx_t* ctx, uint32_t  width, uint32_t  height,
                                      dc_aiq_working_mode_t mode);

/*!
 * \brief start aiq control system
 * should be called after \ref dc_aiq_uapi_sysctl_prepare. After this call,
 * the aiq system repeats getting 3A statistics from ISP driver, running
 * aiq algorimths(AE, AWB, AF, etc.), setting new parameters to drivers.
 *
 * \param[in] ctx             the context returned by \ref dc_aiq_uapi_sysctl_init
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_start(const dc_aiq_sys_ctx_t* ctx);

/*!
 * \brief stop aiq control system
 *
 * \param[in] ctx             the context returned by \ref dc_aiq_uapi_sysctl_init
 * \param[in] keep_ext_hw_st  do not change external devices status, like ircut/cpsl
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_stop(const dc_aiq_sys_ctx_t* ctx, bool keep_ext_hw_st);

XCamReturn dc_aiq_uapi_sysctl_getStaticMetas(const char* sns_ent_name, dc_aiq_static_info_t* static_info);

/*!
 * \brief enum static camera infos
 *
 * \param[in] index           which camera info will be enum
 * \param[out] static_info    returned camera infos
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_enumStaticMetas(int index, dc_aiq_static_info_t* static_info);

/*!
 * \brief get sensor entity name from video node
 * \param[in] vd             pp stream video node full path
 * \return return the binded sensor name
 */
const char* dc_aiq_uapi_sysctl_getBindedSnsEntNmByVd(const char* vd);

XCamReturn dc_aiq_uapi_sysctl_getMetaData(const dc_aiq_sys_ctx_t* ctx, uint32_t frame_id, dc_aiq_metas_t* metas);

#if 0
int32_t dc_aiq_uapi_sysctl_getState(const dc_aiq_sys_ctx_t* ctx);
#endif

XCamReturn dc_aiq_uapi_sysctl_setModuleCtl(const dc_aiq_sys_ctx_t* ctx, dc_aiq_module_id_t mId, bool mod_en);

int32_t dc_aiq_uapi_sysctl_getModuleCtl(const dc_aiq_sys_ctx_t* ctx, dc_aiq_module_id_t mId, bool *mod_en);

/*!
 * \brief register customized algo lib
 *
 * \param[in] ctx context
 * \param[in,out] algo_lib_des allocate a new unique id value for algo_lib_des->id if success.\n
 *                             this id could be used in \ref dc_aiq_uapi_sysctl_unRegLib
 *                             or other lib APIs. The deference object by
 *                             \ref algo_lib_des should be valid until \ref dc_aiq_uapi_sysctl_unRegLib
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_regLib(const dc_aiq_sys_ctx_t* ctx, RkAiqAlgoDesComm* algo_lib_des);

/*!
 * \brief unregister customized algo lib
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          returned by call \ref dc_aiq_uapi_sysctl_regLib
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_unRegLib(const dc_aiq_sys_ctx_t* ctx, const int algo_type, const int lib_id);

/*!
 * \brief enable or disable algo lib
 * If the \ref lib_id is the same as the current running algo, this interface
 * could be called in any state except for the context uninitialized. Otherwise,
 * it could only be called in prepared or initialized state followed by
 * call \ref dc_aiq_uapi_sysctl_prepare , and in this case, the old algo which type
 * is \ref algo_type will be replaced by the new algo \ref lib_id.
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          0 for system integrated algos;\n
 *                            returned by call \ref dc_aiq_uapi_sysctl_regLib for customer algos
 * \param[in] enable          enable or disable algos. enable means running the algo's processes\n
 *                            defined in \ref RkAiqAlgoDesComm; disable means\n
 *                            bypass the algo's prcosses.
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_enableAxlib(const dc_aiq_sys_ctx_t* ctx, const int algo_type,
                                          const int lib_id, bool enable);

/*!
 * \brief get algo lib enabled status
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          0 for system integrated algos;\n
 *                            returned by call \ref dc_aiq_uapi_sysctl_regLib for customer algos
 * \return                    return true if enabled , false if disabled or unregistered
 */
bool dc_aiq_uapi_sysctl_getAxlibStatus(const dc_aiq_sys_ctx_t* ctx, const int algo_type, const int lib_id);

/*!
 * \brief get enabled algo lib context
 * The returned algo context will be used as the first param of algo private APIs.
 * For those customer's algo lib, this interface should be called after
 * \ref dc_aiq_uapi_sysctl_enableAxlib, or will return NULL.
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \return return NULL if failed
 */
RkAiqAlgoContext* dc_aiq_uapi_sysctl_getEnabledAxlibCtx(const dc_aiq_sys_ctx_t* ctx, const int algo_type);

/*!
 * \brief get algo lib context
 * The returned algo context will be used as the first param of algo private APIs.
 * For those customer's algo lib, this interface should be called after
 * \ref dc_aiq_uapi_sysctl_regLib, or will return NULL.
 *
 * \param[in] ctx             context
 * \param[in] algo_type       algo type defined by RkAiqAlgoDesComm.type
 * \param[in] lib_id          0 for system integrated algos;\n
 *                            returned by call \ref dc_aiq_uapi_sysctl_regLib for customer algos
 * \return return NULL if failed
 */
RkAiqAlgoContext* dc_aiq_uapi_sysctl_getAxlibCtx(const dc_aiq_sys_ctx_t* ctx, const int algo_type, const int lib_id);

/*!
 * \brief get 3a stats
 *
 * \param[in] ctx             context
 * \param[out] stats          stats params
 * \return return 0 if success
 * \note non-blocked interface, and copy the result to stats.
 */
XCamReturn dc_aiq_uapi_sysctl_get3AStats(const dc_aiq_sys_ctx_t* ctx, dc_aiq_isp_stats_t *stats);

/*!
 * \brief get 3a stats
 *
 * \param[in] ctx             context
 * \param[out] stats          stats params ref
 * \param[in] timeout_ms      -1 means wait always until stats is available or
 *                            stopped
 * \return return 0 if success
 * \note blocked interface, and return the stats ref, user should
 *       call \ref dc_aiq_uapi_sysctl_release3AStatsRef to release.
 */
XCamReturn dc_aiq_uapi_sysctl_get3AStatsBlk(const dc_aiq_sys_ctx_t* ctx, dc_aiq_isp_stats_t **stats, int timeout_ms);

/*!
 * \brief release 3a stats result ref
 *
 * \param[in] ctx             context
 * \param[out] stats          stats ref
 * \return void
 * \note called with \ref dc_aiq_uapi_sysctl_get3AStatsBlk
 */
void dc_aiq_uapi_sysctl_release3AStatsRef(const dc_aiq_sys_ctx_t* ctx, dc_aiq_isp_stats_t *stats);

/*!
 * \brief set compensation light config
 *
 * \param[in] ctx             context
 * \param[out] cfg            cpsl configs
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_setCpsLtCfg(const dc_aiq_sys_ctx_t* ctx, dc_aiq_cpsl_cfg_t* cfg);

/*!
 * \brief get compensation light info
 *
 * \param[in] ctx             context
 * \param[out] info           current cpsl settings
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_getCpsLtInfo(const dc_aiq_sys_ctx_t* ctx, dc_aiq_cpsl_info_t* info);

/*!
 * \brief query compensation light capability
 *
 * \param[in] ctx             context
 * \param[out] cap            cpsl cap info
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_queryCpsLtCap(const dc_aiq_sys_ctx_t* ctx, dc_aiq_cpsl_cap_t* cap);

/*!
 * \brief prepare dc-raw-format data process environment
 *
 * \param[in] ctx             context
 * \param[in] prop            prepare params
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_preparedcRaw(const dc_aiq_sys_ctx_t* ctx, dc_aiq_raw_prop_t prop);

/*!
 * \brief queue dc-Raw-format buffer into aiq control system
 *
 * \param[in] ctx             context
 * \param[in] rawdata         dc-Raw-format buffer
 * \param[in] sync            sync flag, true means sync mode,calling process will be blocked,
 *                            until the queued frame is processed. false means async mode, calling
 *                            process is not blocked, if you want to free rawdata or reuse it, callback
 *                            should be registered,after frame is processed, callback function would be called.
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_enqueuedcRawBuf(const dc_aiq_sys_ctx_t* ctx, void *rawdata, bool sync);

/*!
 * \brief queue dc-Raw-format file into aiq control system
 *
 * \param[in] ctx             context
 * \param[in] path            dc-Raw-format file path
 * calling process will be blocked until the queued frame is processed
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_enqueuedcRawFile(const dc_aiq_sys_ctx_t* ctx, const char *path);

/*!
 * \brief regist dc-Raw-format buffer callback into aiq control system
 *
 * \param[in] ctx             context
 * \param[in] callback        callback function pointer
 * if callback function is registered,  (when dc_aiq_uapi_sysctl_enqueuedcRawBuf used in sync mode)
 * callback will be called in sync after the queued raw buffer is processed, raw buffer pointer
 * which passed into aiq by dc_aiq_uapi_sysctl_enqueuedcRawBuf would be passed back into the callback
 * function you registered.
 * this function is not required.
 *
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_registdcRawCb(const dc_aiq_sys_ctx_t* ctx, void (*callback)(void*));

/*!
 * \brief set the bypass stream rotation
 *
 * \param[in] ctx             context
 * \param[in] rot             rotation val
 * \return return 0 if success
 */
XCamReturn dc_aiq_uapi_sysctl_setSharpFbcRotation(const dc_aiq_sys_ctx_t* ctx, dc_aiq_rotation_t rot);

/*!
 * \brief get aiq version infos
 *
 * \param[in,out] vers         version infos
 * \return return 0 if success
 */
void dc_aiq_uapi_get_version_info(dc_aiq_ver_info_t* vers);

/*!
 * \brief switch working mode dynamically
 * this aims to switch the isp pipeline working mode fast, and can be called on
 * streaming status. On non streaming status, should call dc_aiq_uapi_sysctl_prepare
 * instead of this to set working mode.
 */
XCamReturn dc_aiq_uapi_sysctl_swWorkingModeDyn(const dc_aiq_sys_ctx_t* ctx, dc_aiq_working_mode_t mode);

/*!
 * \brief set multiple cameras working concurrently
 * Notify this AIQ ctx will run with other sensor's AIQ ctx.

 * \param[in] cc        set cams concurrently used or not
 * \note should be called before dc_aiq_uapi_sysctl_start
 */
void dc_aiq_uapi_sysctl_setMulCamConc(const dc_aiq_sys_ctx_t* ctx, bool cc);

/*!
 * \brief set crop window of isp input
 * This API will affect the isp pipeline resolution.
 *
 * \param[in] rect      set cams crop prop
 * \note Optinal API, should be called before dc_aiq_uapi_sysctl_prepare
 */
XCamReturn dc_aiq_uapi_sysctl_setCrop(const dc_aiq_sys_ctx_t* sys_ctx, dc_aiq_rect_t rect);

/*!
 * \brief get crop window of isp input
 *
 * \param[in] rect       get cams crop prop
 */
XCamReturn dc_aiq_uapi_sysctl_getCrop(const dc_aiq_sys_ctx_t* sys_ctx, dc_aiq_rect_t *rect);

/*!
 * \brief apply an new iq file when stream on
 *
 * \param[in] iqfile       iqfile which will be applied
 */

XCamReturn dc_aiq_uapi_sysctl_updateIq(const dc_aiq_sys_ctx_t* sys_ctx, char* iqfile);

/*!
 * \brief get current used calibDb struct point
 *
 * \param[in] ctx             context
 * \return current used calibDb struct point
 */

CamCalibDbContext_t* dc_aiq_uapi_sysctl_getCurCalib(const dc_aiq_sys_ctx_t* ctx);

/*!
 * \brief update calibdb dynamically
 *
 * \param[in] ctx             context
 * \param[out] calib          original calib point could be got by
 *                            API dc_aiq_uapi_sysctl_getCurCalib
 */

XCamReturn dc_aiq_uapi_sysctl_upateCalib(const dc_aiq_sys_ctx_t* ctx, CamCalibDbContext_t* calib);

/*!
 * \brief get describtion of sensor
 *
 * \param[out] sensorDes
 */
XCamReturn dc_aiq_uapi_sysctl_getSensorDiscrib(const dc_aiq_sys_ctx_t* sys_ctx, dc_aiq_exposure_sensor_descriptor *sensorDes);


/******************************/
/*
**********************************************************
* Exposure
**********************************************************
*/

/*
*****************************
*
* Desc: set exposure control mode
* Argument:
*   mode:  auto: auto exposure
*          manual: manual exposure
*****************************
*/
XCamReturn dc_aiq_uapi_setExpMode(const dc_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn dc_aiq_uapi_getExpMode(const dc_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set exposure mode
* Argument:
*   mode: if mode is set to manual, gain&time initial value will be used
*
*****************************
*/
XCamReturn dc_aiq_uapi_setAeMode(const dc_aiq_sys_ctx_t* ctx, aeMode_t mode);
XCamReturn dc_aiq_uapi_getAeMode(const dc_aiq_sys_ctx_t* ctx, aeMode_t *mode);
/*
*****************************
*
* Desc: set manual exposure
* Argument:
*   gain:
*   time:
*
*****************************
*/
XCamReturn dc_aiq_uapi_setManualExp(const dc_aiq_sys_ctx_t* ctx, float gain, float time);
/*
*****************************
*
* Desc: set exposure parameter
* Argument:
*    auto exposure mode:
*      exposure gain will be adjust between [gain->min, gain->max]；
*    manual exposure mode:
*      gain->min == gain->max
*
*****************************
*/
XCamReturn dc_aiq_uapi_setExpGainRange(const dc_aiq_sys_ctx_t* ctx, paRange_t *gain);
XCamReturn dc_aiq_uapi_getExpGainRange(const dc_aiq_sys_ctx_t* ctx, paRange_t *gain);
/*
*****************************
*
* Desc: set exposure parameter
* Argument:
*    auto exposure mode:
*       exposure time will be adjust between [time->min, time->max]；
*    manual exposure mode:
*       exposure time will be set gain->min == gain->max;
*
*****************************
*/
XCamReturn dc_aiq_uapi_setExpTimeRange(const dc_aiq_sys_ctx_t* ctx, paRange_t *time);
XCamReturn dc_aiq_uapi_getExpTimeRange(const dc_aiq_sys_ctx_t* ctx, paRange_t *time);

/*
**********************************************************
* Auto exposure advanced features
**********************************************************
*/
/*
*****************************
*
* Desc: blacklight compensation
* Argument:
*      on:  1  on
*           0  off
*      areaType: blacklight compensation area
*
*****************************
*/
XCamReturn dc_aiq_uapi_setBLCMode(const dc_aiq_sys_ctx_t* ctx, bool on, aeMeasAreaType_t areaType);

/*
*****************************
*
* Desc: backlight compensation strength,only available in normal mode
* Argument:
*      strength:  [1,100]
*****************************
*/
XCamReturn dc_aiq_uapi_setBLCStrength(const dc_aiq_sys_ctx_t* ctx, int strength);

/*
*****************************
*
* Desc: highlight compensation
* Argument:
*      on:  1  on
*           0  off
*
*****************************
*/
XCamReturn dc_aiq_uapi_setHLCMode(const dc_aiq_sys_ctx_t* ctx, bool on);

/*
*****************************
*
* Desc: highlight compensation strength,only available in normal mode
* Argument:
*      strength:  [1,100]
*****************************
*/
XCamReturn dc_aiq_uapi_setHLCStrength(const dc_aiq_sys_ctx_t* ctx, int strength);

/*
*****************************
*
* Desc: set anti-flicker mode
* Argument:
*    mode
*
*****************************
*/
XCamReturn dc_aiq_uapi_setAntiFlickerMode(const dc_aiq_sys_ctx_t* ctx, antiFlickerMode_t mode);
XCamReturn dc_aiq_uapi_getAntiFlickerMode(const dc_aiq_sys_ctx_t* ctx, antiFlickerMode_t *mode);

/*
*****************************
*
* Desc: set power line frequence
* Argument:
*    freq
*
*****************************
*/
XCamReturn dc_aiq_uapi_setExpPwrLineFreqMode(const dc_aiq_sys_ctx_t* ctx, expPwrLineFreq_t freq);
XCamReturn dc_aiq_uapi_getExpPwrLineFreqMode(const dc_aiq_sys_ctx_t* ctx, expPwrLineFreq_t *freq);

/*
**********************************************************
* White balance & Color
**********************************************************
*/

/*
*****************************
*
* Desc: set white balance mode
* Argument:
*   mode:  auto: auto white balance
*          manual: manual white balance
*****************************
*/
XCamReturn dc_aiq_uapi_setWBMode(const dc_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn dc_aiq_uapi_getWBMode(const dc_aiq_sys_ctx_t* ctx, opMode_t *mode);


/*
*****************************
*
* Desc: lock/unlock auto white balance
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_lockAWB(const dc_aiq_sys_ctx_t* ctx);
XCamReturn dc_aiq_uapi_unlockAWB(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set manual white balance scene mode
* Argument:
*   ct_scene:
*
*****************************
*/
XCamReturn dc_aiq_uapi_setMWBScene(const dc_aiq_sys_ctx_t* ctx, dc_aiq_wb_scene_t scene);
XCamReturn dc_aiq_uapi_getMWBScene(const dc_aiq_sys_ctx_t* ctx, dc_aiq_wb_scene_t *scene);


/*
*****************************
*
* Desc: set manual white balance r/b gain
* Argument:
*   ct_scene:
*
*****************************
*/
XCamReturn dc_aiq_uapi_setMWBGain(const dc_aiq_sys_ctx_t* ctx, dc_aiq_wb_gain_t *gain);
XCamReturn dc_aiq_uapi_getMWBGain(const dc_aiq_sys_ctx_t* ctx, dc_aiq_wb_gain_t *gain);

/*
*****************************
*
* Desc: set manual white balance color temperature
* Argument:
*   ct: color temperature value [2800, 7500]K
*
*****************************
*/
XCamReturn dc_aiq_uapi_setMWBCT(const dc_aiq_sys_ctx_t* ctx, unsigned int ct);
XCamReturn dc_aiq_uapi_getMWBCT(const dc_aiq_sys_ctx_t* ctx, unsigned int *ct);


/*
*****************************
*
* Desc: set color supperssion level
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn dc_aiq_uapi_setCrSuppsn(const dc_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn dc_aiq_uapi_getCrSuppsn(const dc_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
**********************************************************
* Focus & Zoom
**********************************************************
*/
/*
*****************************
*
* Desc: set focus mode
* Argument:
*   mode:  auto: auto focus
*          manual: manual focus
*          semi-auto: semi-auto focus
*****************************
*/
XCamReturn dc_aiq_uapi_setFocusMode(const dc_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn dc_aiq_uapi_getFocusMode(const dc_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set focus window
* Argument:
*
*****************************
*/
XCamReturn dc_aiq_uapi_setFocusWin(const dc_aiq_sys_ctx_t* ctx, paRect_t *rect);
XCamReturn dc_aiq_uapi_getFocusWin(const dc_aiq_sys_ctx_t* ctx, paRect_t *rect);

/*
*****************************
*
* Desc: set focus meas config
* Argument:
*
*****************************
*/
XCamReturn dc_aiq_uapi_setFocusMeasCfg(const dc_aiq_sys_ctx_t* ctx, dc_aiq_af_algo_meas_t* meascfg);
XCamReturn dc_aiq_uapi_getFocusMeasCfg(const dc_aiq_sys_ctx_t* ctx, dc_aiq_af_algo_meas_t* meascfg);

/*
*****************************
*
* Desc: set fix mode code
* Argument:
*
*****************************
*/
XCamReturn dc_aiq_uapi_setFixedModeCode(const dc_aiq_sys_ctx_t* ctx, short code);
XCamReturn dc_aiq_uapi_getFixedModeCode(const dc_aiq_sys_ctx_t* ctx, short *code);

/*
*****************************
*
* Desc: lock/unlock auto focus
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_lockFocus(const dc_aiq_sys_ctx_t* ctx);
XCamReturn dc_aiq_uapi_unlockFocus(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: oneshot focus
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_oneshotFocus(const dc_aiq_sys_ctx_t* ctx);
XCamReturn dc_aiq_uapi_trackingFocus(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: ManualTriger focus
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_manualTrigerFocus(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: vcm config
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_setVcmCfg(const dc_aiq_sys_ctx_t* ctx, dc_aiq_lens_vcmcfg* cfg);
XCamReturn dc_aiq_uapi_getVcmCfg(const dc_aiq_sys_ctx_t* ctx, dc_aiq_lens_vcmcfg* cfg);

/*
*****************************
*
* Desc: af serach path record
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_getSearchPath(const dc_aiq_sys_ctx_t* ctx, dc_aiq_af_sec_path_t* path);

/*
*****************************
*
* Desc: af serach path record
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_getSearchResult(const dc_aiq_sys_ctx_t* ctx, dc_aiq_af_result_t* result);

/*
*****************************
*
* Desc: Focus Correstion
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_FocusCorrestion(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: Zoom
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_getZoomRange(const dc_aiq_sys_ctx_t* ctx, dc_aiq_af_zoomrange* range);
XCamReturn dc_aiq_uapi_getFocusRange(const dc_aiq_sys_ctx_t* ctx, dc_aiq_af_focusrange* range);
XCamReturn dc_aiq_uapi_setOpZoomPosition(const dc_aiq_sys_ctx_t* ctx, int pos);
XCamReturn dc_aiq_uapi_getOpZoomPosition(const dc_aiq_sys_ctx_t* ctx, int *pos);
XCamReturn dc_aiq_uapi_endOpZoomChange(const dc_aiq_sys_ctx_t* ctx);
XCamReturn dc_aiq_uapi_startZoomCalib(const dc_aiq_sys_ctx_t* ctx);
XCamReturn dc_aiq_uapi_resetZoom(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: Zoom Correstion
* Argument:
*
*
*****************************
*/
XCamReturn dc_aiq_uapi_ZoomCorrestion(const dc_aiq_sys_ctx_t* ctx);

/*
**********************************************************
* HDR
**********************************************************
*/
/*
*****************************
*
* Desc: set hdr mode
* Argument:
*   mode:
*     auto: auto hdr mode
*     manual：manual hdr mode
*
*****************************
*/
XCamReturn dc_aiq_uapi_setHDRMode(const dc_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn dc_aiq_uapi_getHDRMode(const dc_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set manual hdr strength
*    this function is active for HDR is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn dc_aiq_uapi_setMHDRStrth(const dc_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn dc_aiq_uapi_getMHDRStrth(const dc_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
**********************************************************
* Noise reduction
**********************************************************
*/
/*
*****************************
*
* Desc: set/get noise reduction mode
* Argument:
*   mode:
*     auto: auto noise reduction
*     manual：manual noise reduction
*
*****************************
*/
XCamReturn dc_aiq_uapi_setNRMode(const dc_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn dc_aiq_uapi_getNRMode(const dc_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set/get normal noise reduction strength
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn dc_aiq_uapi_setANRStrth(const dc_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn dc_aiq_uapi_getANRStrth(const dc_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: set manual spatial noise reduction strength
*    this function is active for NR is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn dc_aiq_uapi_setMSpaNRStrth(const dc_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn dc_aiq_uapi_getMSpaNRStrth(const dc_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
*****************************
*
* Desc: set/get manual time noise reduction strength
*     this function is active for NR is manual mode
* Argument:
*   level: [0, 100]
*
*****************************
*/
XCamReturn dc_aiq_uapi_setMTNRStrth(const dc_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn dc_aiq_uapi_getMTNRStrth(const dc_aiq_sys_ctx_t* ctx, bool *on, unsigned int *level);

/*
**********************************************************
* Dehaze
**********************************************************
*/
/*
*****************************
*
* Desc: set/get dehaze mode
* Argument:
*   mode:
*     auto: auto dehaze, when use auto, equal use dc_aiq_uapi_enableDhz
*     manual: Manual dehaze, when needs to use manual, please use dc_aiq_uapi_setMDhzStrth
*
*****************************
*/
XCamReturn dc_aiq_uapi_setDhzMode(const dc_aiq_sys_ctx_t* ctx, opMode_t mode);
XCamReturn dc_aiq_uapi_getDhzMode(const dc_aiq_sys_ctx_t* ctx, opMode_t *mode);

/*
*****************************
*
* Desc: set/get manual dehaze strength
*     this function is active for dehaze is manual mode
* Argument:
*   level: [0, 10]
*   Do not need to use dc_aiq_uapi_enableDhz and dc_aiq_uapi_setDhzMode before use this
*
*****************************
*/
XCamReturn dc_aiq_uapi_setMDhzStrth(const dc_aiq_sys_ctx_t* ctx, bool on, unsigned int level);
XCamReturn dc_aiq_uapi_getMDhzStrth(const dc_aiq_sys_ctx_t* ctx, bool* on, unsigned int* level);

/*
*****************************
*
* Desc: enable dehaze
* Argument:
*   When dehaze enable, dehaze on and para use use IQ xml
*   When dehaze disable, dehaze off and enhance para use use IQ xml
*
*****************************
*/
XCamReturn dc_aiq_uapi_enableDhz(const dc_aiq_sys_ctx_t* ctx);
XCamReturn dc_aiq_uapi_disableDhz(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set/get dadc area boost strength
*    this function is active for normal mode
* Argument:
*   level: [1, 10]
*
*****************************
*/
XCamReturn dc_aiq_uapi_setDarkAreaBoostStrth(const dc_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn dc_aiq_uapi_getDarkAreaBoostStrth(const dc_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
**********************************************************
* Image adjust
**********************************************************
*/

/*
*****************************
*
* Desc: Adjust image contrast level
* Argument:
*    level: contrast level, [0, 255]
*****************************
*/
XCamReturn dc_aiq_uapi_setContrast(const dc_aiq_sys_ctx_t* ctx, unsigned int level);

/*
*****************************
*
* Desc: Adjust image brightness level
* Argument:
*    level: brightness level, [0, 255]
*****************************
*/
XCamReturn dc_aiq_uapi_setBrightness(const dc_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn dc_aiq_uapi_getBrightness(const dc_aiq_sys_ctx_t* ctx, unsigned int *level);
/*
*****************************
*
* Desc: Adjust image saturation level
* Argument:
*    level: saturation level, [0, 255]
*****************************
*/
XCamReturn dc_aiq_uapi_setSaturation(const dc_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn dc_aiq_uapi_getSaturation(const dc_aiq_sys_ctx_t* ctx, unsigned int* level);
/*
*****************************
*
* Desc: Adjust image hue level
* Argument:
*    level: hue level, [0, 255]
*****************************
*/
XCamReturn dc_aiq_uapi_setHue(const dc_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn dc_aiq_uapi_getHue(const dc_aiq_sys_ctx_t* ctx, unsigned int *level);
/*
*****************************
*
* Desc: Adjust image sharpness level
* Argument:
*    level: sharpness level, [0, 100]
*****************************
*/
XCamReturn dc_aiq_uapi_setSharpness(const dc_aiq_sys_ctx_t* ctx, unsigned int level);
XCamReturn dc_aiq_uapi_getSharpness(const dc_aiq_sys_ctx_t* ctx, unsigned int *level);

/*
*****************************
*
* Desc: Adjust image gamma level
* Argument:
*    level: gamma level, [0, 100]
*****************************
*/
XCamReturn dc_aiq_uapi_setGammaCoef(const dc_aiq_sys_ctx_t* ctx, dc_aiq_gamma_attrib_t gammaAttr);

/*
*****************************
*
* Desc: set gray mode
* Argument:
*    mode : dc_AIQ_GRAY_MODE_CPSL
*           dc_AIQ_GRAY_MODE_OFF
*           dc_AIQ_GRAY_MODE_ON
* related: dc_aiq_uapi_sysctl_setCpsLtCfg
*****************************
*/
XCamReturn dc_aiq_uapi_setGrayMode(const dc_aiq_sys_ctx_t* ctx, dc_aiq_gray_mode_t mode);
dc_aiq_gray_mode_t dc_aiq_uapi_getGrayMode(const dc_aiq_sys_ctx_t* ctx);

/*
*****************************
*
* Desc: set frame rate
* Argument:
*    frameRateInfo_t :
*           auto mode: vary fps
*         manual mode: fixed fps
*****************************
*/
XCamReturn dc_aiq_uapi_setFrameRate(const dc_aiq_sys_ctx_t* ctx, frameRateInfo_t info);
XCamReturn dc_aiq_uapi_getFrameRate(const dc_aiq_sys_ctx_t* ctx, frameRateInfo_t *info);

/*
*****************************
*
* Desc: set mirro & flip
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setMirroFlip(const dc_aiq_sys_ctx_t* ctx,
                                    bool mirror,
                                    bool flip,
                                    int skip_frm_cnt);

/*
*****************************
*
* Desc: get mirro & flip
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_getMirrorFlip(const dc_aiq_sys_ctx_t* ctx, bool* mirror, bool* flip);

/*
*****************************
*
* Desc: fec dynamic switch, valid only if aiq hasn't executed the 'prepare' action
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setFecEn(const dc_aiq_sys_ctx_t* ctx, bool en);

/*
*****************************
*
* Desc: set corrective direction of FEC, valid only if aiq hasn't executed the 'prepare' action
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setFecCorrectDirection(const dc_aiq_sys_ctx_t* ctx,
        const fec_correct_direction_t direction);
/*
*****************************
*
* Desc: The FEC module is still working in bypass state
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setFecBypass(const dc_aiq_sys_ctx_t* ctx, bool en);

/*
*****************************
*
* Desc: the adjustment range of distortion intensity is 0~255
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setFecCorrectLevel(const dc_aiq_sys_ctx_t* ctx, int correctLevel);

/*
*****************************
*
* Desc:
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setFecCorrectMode(const dc_aiq_sys_ctx_t* ctx,
        const fec_correct_mode_t mode);

/*
*****************************
*
* Desc:
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setLdchEn(const dc_aiq_sys_ctx_t* ctx, bool en);
/*
*****************************
*
* Desc: the adjustment range of distortion intensity is 0~255
* Argument:
*****************************
*/
XCamReturn dc_aiq_uapi_setLdchCorrectLevel(const dc_aiq_sys_ctx_t* ctx, int correctLevel);


/**********/
XCamReturn dc_aiq_user_api_ae_queryExpResInfo(const rk_aiq_sys_ctx_t* ctx, Uapi_ExpQueryInfo_t* pExpResInfo);

XCamReturn dc_aiq_user_api_awb_GetCCT(const rk_aiq_sys_ctx_t* sys_ctx, rk_aiq_wb_cct_t *cct);

XCamReturn dc_aiq_user_api_ae_getExpSwAttr(const rk_aiq_sys_ctx_t* ctx, Uapi_ExpSwAttr_t* pExpSwAttr);
XCamReturn dc_aiq_user_api_ae_setExpSwAttr(const rk_aiq_sys_ctx_t* ctx, const Uapi_ExpSwAttr_t expSwAttr);
#endif





