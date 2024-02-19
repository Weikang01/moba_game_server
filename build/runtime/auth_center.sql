CREATE DATABASE `auth_center` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

CREATE TABLE `user_info` (
  `uid` int(11) unsigned zerofill NOT NULL AUTO_INCREMENT COMMENT 'unique UID of user',
  `unick` varchar(32) NOT NULL COMMENT 'user''s nickname',
  `usex` int DEFAULT '0' COMMENT '0: unknown, 1:male, 2:female',
  `usysavatar` int DEFAULT '0' COMMENT 'system default avatar',
  `uname` varchar(32) DEFAULT '' COMMENT 'user''s account name',
  `upwd` varchar(32) DEFAULT '' COMMENT 'MD5 value of user''s password',
  `uvip` int DEFAULT '0' COMMENT 'user''s VIP level, default 0',
  `vip_expire_time` int DEFAULT NULL COMMENT 'user''s VIP expire time''s timestamp',
  `email` varchar(64) DEFAULT NULL COMMENT 'user''s email',
  `is_guest` int DEFAULT NULL COMMENT 'whether account is a guest or not',
  `guest_key` varchar(64) DEFAULT NULL COMMENT 'unique key of guest account',
  `status` int unsigned NOT NULL DEFAULT '0' COMMENT 'user status, 0: normal, others: depend on the requirements',
  PRIMARY KEY (`uid`),
  UNIQUE KEY `uid_UNIQUE` (`uid`) /*!80000 INVISIBLE */
) ENGINE=InnoDB AUTO_INCREMENT=35 DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='save user info';

