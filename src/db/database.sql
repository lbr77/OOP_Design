CREATE DATABASE IF NOT EXISTS `test` CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci;

USE `test`;

CREATE TABLE IF NOT EXISTS `users_qq` (
    id BIGINT PRIMARY KEY AUTO_INCREMENT,
    username VARCHAR(255) NOT NULL,
    password VARCHAR(255) NOT NULL,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP,
    birthday DATE,
    address VARCHAR(255),
);
CREATE TABLE IF NOT EXISTS chats_qq (
    id BIGINT PRIMARY KEY AUTO_INCREMENT, -- 聊天 ID
    type_ ENUM('private', 'group') NOT NULL, -- 聊天类型
    title VARCHAR(100),                  -- 群组或频道标题（私人聊天可为空）
    description TEXT,                    -- 聊天描述（仅适用于群组或频道）
    creator_id BIGINT NOT NULL,          -- 创建者 ID
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 创建时间
    FOREIGN KEY (creator_id) REFERENCES users_qq(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS chat_members_qq (
    chat_id BIGINT NOT NULL,             -- 聊天 ID
    user_id BIGINT NOT NULL,             -- 用户 ID
    role ENUM('member', 'admin', 'owner') DEFAULT 'member', -- 用户角色
    joined_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 加入时间
    PRIMARY KEY (chat_id, user_id),
    FOREIGN KEY (chat_id) REFERENCES chats_qq(id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users_qq(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS messages_qq (
    id BIGINT PRIMARY KEY AUTO_INCREMENT, -- 消息 ID
    chat_id BIGINT NOT NULL,              -- 聊天 ID
    sender_id BIGINT NOT NULL,            -- 发送者 ID
    content TEXT,                         -- 消息内容
    message_type ENUM('text', 'image', 'file') DEFAULT 'text', -- 消息类型
    sent_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP, -- 发送时间
    is_deleted BOOLEAN DEFAULT FALSE,     -- 是否已删除
    FOREIGN KEY (chat_id) REFERENCES chats_qq(id) ON DELETE CASCADE,
    FOREIGN KEY (sender_id) REFERENCES users_qq(id) ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS message_status_qq (
    message_id BIGINT NOT NULL,           -- 消息 ID
    user_id BIGINT NOT NULL,              -- 用户 ID
    is_read BOOLEAN DEFAULT FALSE,        -- 是否已读
    read_at TIMESTAMP NULL,               -- 阅读时间
    PRIMARY KEY (message_id, user_id),
    FOREIGN KEY (message_id) REFERENCES messages_qq(id) ON DELETE CASCADE,
    FOREIGN KEY (user_id) REFERENCES users_qq(id) ON DELETE CASCADE
);