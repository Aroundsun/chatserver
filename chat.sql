--群组表 存储所有的群组的信息
DROP TABLE IF EXISTS `allgroup`;
CREATE TABLE `allgroup` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `groupname` varchar(50) CHARACTER SET latin1 NOT NULL,
  `groupdesc` varchar(200) CHARACTER SET latin1 DEFAULT '',
  PRIMARY KEY (`id`),
  UNIQUE KEY `groupname` (`groupname`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

--好友表 -存储用户的好友信息
DROP TABLE IF EXISTS `friend`;
CREATE TABLE `friend` (
  `userid` int(11) NOT NULL,
  `friendid` int(11) NOT NULL,
  KEY `userid` (`userid`,`friendid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--群组成员表 -存储加入群组的用户的信息
DROP TABLE IF EXISTS `groupuser`;
CREATE TABLE `groupuser` (
  `groupid` int(11) NOT NULL,
  `userid` int(11) NOT NULL,
  `grouprole` enum('creator','normal') CHARACTER SET latin1 DEFAULT NULL,
  KEY `groupid` (`groupid`,`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--离线消息表 -存储离线消息
DROP TABLE IF EXISTS `offlinemessage`;
CREATE TABLE `offlinemessage` (
  `userid` int(11) NOT NULL,
  `message` varchar(500) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--用户表 -存储用户的基本信息
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `name` varchar(50) DEFAULT NULL,
  `password` varchar(50) DEFAULT NULL,
  `state` enum('online','offline') CHARACTER SET latin1 DEFAULT 'offline',
  PRIMARY KEY (`id`),
  UNIQUE KEY `name` (`name`)
) ENGINE=InnoDB AUTO_INCREMENT=22 DEFAULT CHARSET=utf8;


