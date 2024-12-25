CREATE DATABASE IF NOT EXISTS `database` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE `database`;
CREATE TABLE users (
    id BIGINT AUTO_INCREMENT PRIMARY KEY, -- 唯一用户ID
    nickname VARCHAR(50) NOT NULL,        -- 昵称
    password CHAR(64) NOT NULL,           -- 密码
    birthdate DATE,                       -- 出生日期
    created_at DATETIME DEFAULT NOW(), -- 注册时间
    location VARCHAR(100),                -- 所在地
    platform ENUM('QQ', 'WeChat') NOT NULL, -- 平台类型（如QQ或微信）
    bind_id BIGINT,                  -- QQ ID（可为空，与微信绑定时填写）
    status TINYINT DEFAULT 1              -- 用户状态，1为活跃，0为停用
);

CREATE TABLE friends (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    user_id BIGINT NOT NULL,             -- 用户ID
    friend_id BIGINT NOT NULL,           -- 好友ID
    created_at DATETIME DEFAULT NOW(),   -- 好友关系创建时间
    status ENUM('pending', 'active', 'blocked') DEFAULT 'pending', -- 好友状态
    UNIQUE(user_id, friend_id),          -- 防止重复添加好友
    FOREIGN KEY (user_id) REFERENCES users(id),
    FOREIGN KEY (friend_id) REFERENCES users(id)
);

CREATE TABLE groups (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    group_name VARCHAR(100) NOT NULL,    -- 群名
    platform ENUM('QQ', 'WeChat') NOT NULL, -- 平台类型
    owner_id BIGINT NOT NULL,            -- 群主ID
    created_at DATETIME DEFAULT NOW(),   -- 创建时间
    FOREIGN KEY (owner_id) REFERENCES users(id)
);

CREATE TABLE group_members (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    group_id BIGINT NOT NULL,            -- 群ID
    user_id BIGINT NOT NULL,             -- 用户ID
    role ENUM('pending','member', 'admin', 'owner') DEFAULT 'pending', -- 角色
    joined_at DATETIME DEFAULT NOW(),    -- 加入时间
    FOREIGN KEY (group_id) REFERENCES groups(id),
    FOREIGN KEY (user_id) REFERENCES users(id)
);

CREATE TABLE messages (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    sender_id BIGINT NOT NULL,           -- 发送者ID
    receiver_id BIGINT,                  -- 接收者ID（点对点聊天）
    group_id BIGINT,                     -- 群ID（群聊）
    content TEXT NOT NULL,               -- 消息内容
    sent_at DATETIME DEFAULT NOW(),      -- 发送时间
    message_type ENUM('private', 'group') NOT NULL, -- 消息类型
    FOREIGN KEY (sender_id) REFERENCES users(id),
    FOREIGN KEY (receiver_id) REFERENCES users(id),
    FOREIGN KEY (group_id) REFERENCES groups(id)
);

CREATE TABLE is_read (
    id BIGINT AUTO_INCREMENT PRIMARY KEY,
    message_id BIGINT NOT NULL,          -- 消息ID
    user_id BIGINT NOT NULL,             -- 用户ID
    read_at DATETIME DEFAULT NOW(),      -- 阅读时间
    FOREIGN KEY (message_id) REFERENCES messages(id),
    FOREIGN KEY (user_id) REFERENCES users(id)
)