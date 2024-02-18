CREATE SCHEMA `auth_center`;

CREATE TABLE `auth_center`.`user_info` (
  `uid` INT(11) UNSIGNED ZEROFILL NOT NULL AUTO_INCREMENT COMMENT 'unique UID of user',
  `unick` VARCHAR(32) NOT NULL COMMENT 'user\'s nickname',
  `usex` INT(8) NULL DEFAULT 0 COMMENT '0: unknown, 1:male, 2:female',
  `usysavatar` INT(8) NULL DEFAULT 0 COMMENT 'system default avatar',
  `uname` VARCHAR(32) NULL DEFAULT '' COMMENT 'user\'s account name', -- Changed to NULL and added default
  `upwd` VARCHAR(32) NULL DEFAULT '' COMMENT 'MD5 value of user\'s password', -- Changed to NULL and added default
  `uvip` INT(8) NULL DEFAULT 0 COMMENT 'user\'s VIP level, default 0',
  `vip_expire_time` INT(32) NULL COMMENT 'user\'s VIP expire time\'s timestamp',
  `email` VARCHAR(64) NULL COMMENT 'user\'s email',
  `is_guest` INT(8) NULL COMMENT 'whether account is a guest or not',
  `guest_key` VARCHAR(64) NULL COMMENT 'unique key of guest account',
  `status` INT(8) NOT NULL DEFAULT 0 COMMENT 'user status, 0: normal, others: depend on the requirements',
  PRIMARY KEY (`uid`),
  UNIQUE INDEX `uid_UNIQUE` (`uid` ASC) INVISIBLE
) COMMENT = 'save user info';

