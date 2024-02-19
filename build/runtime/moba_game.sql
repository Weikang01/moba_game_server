CREATE DATABASE `moba_game` /*!40100 DEFAULT CHARACTER SET utf8mb4 COLLATE utf8mb4_0900_ai_ci */ /*!80016 DEFAULT ENCRYPTION='N' */;

CREATE TABLE `ugame` (
  `id` int unsigned NOT NULL AUTO_INCREMENT COMMENT 'unique id',
  `uid` int DEFAULT NULL COMMENT 'unique id of the player',
  `uchip` int DEFAULT '0' COMMENT 'user''s number of gold coin',
  `uchip2` int DEFAULT '0' COMMENT 'user''s other currency (reserved)',
  `uchip3` int DEFAULT '0' COMMENT 'user''s other currency (reserved)',
  `uvip` int DEFAULT '0' COMMENT 'user''s VIP level within the game',
  `uvip_endtime` int DEFAULT '0' COMMENT 'endtime of uVIP',
  `udata1` int DEFAULT '0' COMMENT 'user''s ingame item 1',
  `udata2` int DEFAULT '0' COMMENT 'user''s ingame item 2',
  `udata3` int DEFAULT '0' COMMENT 'user''s ingame item 3',
  `uexp` int DEFAULT '0' COMMENT 'user''s experience',
  `ustatus` int DEFAULT '0' COMMENT 'user status. 0: normal, other reserved (blocked, etc)',
  PRIMARY KEY (`id`),
  UNIQUE KEY `id_UNIQUE` (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci COMMENT='save player''s primary in-game data in game of "Moba"\nsuch as gold, other currency, item, in-game vip level, player''s experience\nuid: player identifier\nid: auto-incremented unique id';
