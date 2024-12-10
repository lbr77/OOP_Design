CREATE DATABASE IF NOT EXISTS `test` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE `test`;

CREATE TABLE IF NOT EXISTS `users_qq` (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    birthday DATE,
    address VARCHAR(255),
    bind_wx_id BIGINT
);
insert into users_qq (username,password) values('admin','21232f297a57a5a743894a0e4a801fc3');