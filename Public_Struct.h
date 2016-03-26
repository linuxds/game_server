/*
 *  Created on: Dec 16, 2015
 *      Author: zhangyalei
 */


#ifndef PUBLIC_STURCT_H_
#define PUBLIC_STURCT_H_

#include "boost/unordered_map.hpp"
#include "Time_Value.h"
#include "Misc.h"
#include "Game_Struct.h"

enum Server_Type {
	GAME_SERVER = 1,
	MASTER_SERVER = 2,
};

struct Cid_Info {
	int gate_cid;
	int player_cid;

	Cid_Info(void) : gate_cid(0), player_cid(0) {}
	Cid_Info(int gate_cid_, int player_cid_) : gate_cid(gate_cid_), player_cid(player_cid_) {};
};

struct Saving_Info {
	int64_t role_id;		// 角色
	Time_Value timestamp;	// 保存时的时间错

	Saving_Info(void) : role_id(0) {}
	Saving_Info(int64_t role_id_, Time_Value timestamp_) : role_id(role_id_), timestamp(timestamp_) {};
};

struct Close_Info {
	int cid;
	Time_Value timestamp;

	Close_Info(void) : cid(-1), timestamp(Time_Value::zero) { }
	Close_Info(int p_cid, const Time_Value &p_timestamp) : cid(p_cid), timestamp(p_timestamp) { }
};

struct Msg_Info {
	bool is_inited;
	int cid;
	long hash_key;								/// 用于加解密的hash key
	uint32_t msg_serial;					/// 上一条消息序号
	Time_Value msg_timestamp;			/// 上一条消息时间戳
	uint32_t msg_interval_count_;	/// 操作频率统计
	Time_Value msg_interval_timestamp;

	void reset(void) {
		is_inited = false;
		cid = -1;
		hash_key = 0;
		msg_serial = 0;
		msg_timestamp = Time_Value::zero;
		msg_interval_count_ = 0;
		msg_interval_timestamp = Time_Value::zero;
	}
};

struct Tick_Info {
	const Time_Value server_info_interval_tick;
	Time_Value server_info_last_tick;

	const Time_Value player_interval_tick; /// Game_Player定时器间隔
	Time_Value player_last_tick;

	const Time_Value saving_scanner_interval_tick;	// 玩家下线保存表的扫描
	Time_Value saving_scanner_last_tick;

	const Time_Value object_pool_interval_tick;
	Time_Value object_pool_last_tick;

	Tick_Info(void)
	: server_info_interval_tick(Time_Value(300, 0)),
	  server_info_last_tick(Time_Value::zero),
	  player_interval_tick(Time_Value(0, 500 * 1000)),
	  player_last_tick(Time_Value::zero),
	  saving_scanner_interval_tick(20, 0),
	  saving_scanner_last_tick(Time_Value::zero),
	  object_pool_interval_tick(300, 0),
	  object_pool_last_tick(Time_Value::zero)
	{ }
};

struct Recycle_Tick {
	enum {
		NON_RECYCLE,	/// 非回收状态
		RECYCLE,		/// 回收状态
	};

	int status;
	Time_Value recycle_tick;

	Recycle_Tick(void): status(NON_RECYCLE) {}

	void reset(void) {
		set(NON_RECYCLE);
	}

	void set(int s) {
		status = s;
		recycle_tick = Time_Value::gettimeofday() + Time_Value(2, 0);
	}
};

struct Player_Data {
	enum	 {
		NULL_STATUS = 0,
		SUCCESS_LOADED,			/// 加载成功
		SUCCESS_CREATED,		/// 创建成功
		ROLE_NOT_EXIST,			/// 角色不存在
		ROLE_HAS_EXIST,			///	角色已经存在
		ROLE_SAVE_OFFLINE,	/// 角色下线保存
	};

	enum {
		PLAYER_CHANGE = 1,
		HERO_CHANGE,
		BAG_CHANGE,
		MAIL_CHANGE,
		SHOP_CHANGE,
		CHANGE_END
	};

	int8_t status;
	std::vector<int32_t> change_module;

	Player_Info player_info;
	Hero_Info hero_info;
	Bag_Info bag_info;
	Mail_Info mail_info;
	Shop_Info shop_info;

	Player_Data(void);
	int serialize(Block_Buffer &buffer) const;
	int deserialize(Block_Buffer &buffer);
	int load(int64_t role_id);
	int save(void);
	void reset(void);
	void set_all_change(bool is_change);
	void set_change(int change_id);
};

struct DB_Cache {
	const static int player_cache_map_bucket_num = 50000;

	DB_Cache(void)
	: id_player_cache_map(player_cache_map_bucket_num),
	  account_player_cache_map(player_cache_map_bucket_num)
	{ }

	boost::unordered_map<int64_t,Player_DB_Cache> id_player_cache_map;
	boost::unordered_map<std::string,Player_DB_Cache> account_player_cache_map;
};

#endif /* PUBLIC_STURCT_H_ */
