<template>
  <div class="min-h-screen bg-gray-100 flex items-center justify-center p-4">
    <div
      class="max-w-6xl w-full max-h-[800px] min-h-[800px] min-w-[320px] h-full bg-white shadow-xl rounded-lg flex overflow-hidden">
      <!-- 侧边栏 -->
      <div class="w-1/4 min-w-[200px] max-w-[300px] bg-indigo-800 text-white">
        <!-- 用户信息 -->
        <div class="relative p-4 flex items-center space-x-3">
          <div @click="openUserProfilePanel"
            class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
            <UserIcon class="h-6 w-6" />
          </div>
          <div>
            <p class="font-semibold">{{ user.username }}</p>
          </div>
          <div class="flex space-x-2 ml-20">
            <button @click="openAddActionModal('addFriend')" class="text-white hover:text-indigo-200" aria-label="添加好友">
              <UserPlusIcon class="h-5 w-5" />
            </button>
            <button @click="openAddActionModal('createGroup')" class="text-white hover:text-indigo-200"
              aria-label="创建群组">
              <UsersIcon class="h-5 w-5" />
            </button>
            <button @click="openSearchPanel" class="text-white hover:text-indigo-200" aria-label="搜索">
              <SearchIcon class="h-5 w-5" />
            </button>
            <button class="text-red-200 hover:text-red-500" aria-label="退出" @click="()=>{
                wx('logout').then(res=>{
                  if(res.code == 200){
                    router.push('/wx/login')
                  }
                })
              }">
              <LogOutIcon class="h-5 w-5" />
            </button>
          </div>

        </div>

        <!-- 标签页切换 -->
        <div class="flex border-b border-indigo-700">
          <button @click="activeTab = 'messages'"
            :class="['flex-1 py-2 text-center', activeTab === 'messages' ? 'bg-indigo-700' : '']">
            消息
          </button>
          <button @click="activeTab = 'friends'"
            :class="['flex-1 py-2 text-center', activeTab === 'friends' ? 'bg-indigo-700' : '']">
            好友
          </button>
          <button @click="activeTab = 'group'"
            :class="['flex-1 py-2 text-center', activeTab === 'group' ? 'bg-indigo-700' : '']">
            群聊
          </button>
        </div>

        <!-- 消息列表 -->
        <div v-if="activeTab === 'messages'" class="overflow-y-auto h-[calc(100%-120px)] min-h-[400px]">
          <div class="p-3 hover:bg-indigo-700 cursor-pointer" @click="curChat = {name: '通知',message_type: 'notice'}">
            <div class="flex items-center space-x-3">
              <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
                <CircleAlertIcon class="h-6 w-6" />
              </div>
              <div class="flex-1">
                <p class="font-semibold">通知</p>
                <p class="text-xs text-indigo-300 truncate"></p>
              </div>
            </div>
          </div>
          <div v-for="(chat,index) in chats" :key="index" class="p-3 hover:bg-indigo-700 cursor-pointer"
            @click="curChat = chat">
            <div class="flex items-center space-x-3">
              <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
                <UserIcon v-if="chat.message_type === 'private'" class="h-6 w-6" />
                <UsersIcon v-else class="h-6 w-6" />
              </div>
              <div class="flex-1">
                <p class="font-semibold">{{ chat.name }}</p>
                <p class="text-xs text-indigo-300 truncate">{{ chat.content }}</p>
              </div>
            </div>
          </div>
        </div>

        <!-- 好友列表 -->
        <div v-if="activeTab === 'friends'" class="overflow-y-auto h-[calc(100%-120px)] min-h-[400px]">
          <div v-for="friend in friends" :key="friend.id" class="p-3 hover:bg-indigo-700 cursor-pointer" @click="() => {
              curChat.sender_id = friend.id
              curChat.message_type = 'private'
              curChat.name = friend.username
            }">
            <div class="flex items-center space-x-3">
              <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
                <UserIcon class="h-6 w-6" />
              </div>
              <div>
                <p class="font-semibold">{{ friend.username }}</p>
                <p class="text-xs text-indigo-300">账户创建了 {{ Math.round((new Date() - new Date(friend.created_at)) / 1000
                  / 60
                  / 60 / 24) }} 天</p>
              </div>
            </div>
          </div>
        </div>
        <!-- 群列表 -->
        <div v-if="activeTab === 'group'" class="overflow-y-auto h-[calc(100%-120px)] min-h-[400px]">
          <div v-for="group in groups" :key="group.id" class="p-3 hover:bg-indigo-700 cursor-pointer"
            @click="curChat.group_id = group.id ; curChat.message_type = 'group' ; curChat.name = group.group_name">
            <div class="flex items-center space-x-3">
              <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
                <UserIcon class="h-6 w-6" />
              </div>
              <div>
                <p class="font-semibold">{{ group.group_name }}</p>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- 聊天区域 -->
      <div v-if="!isSearchPanelOpen" class="flex-1 flex flex-col">
        <!-- 聊天框头部 -->
        <div class="bg-white shadow px-6 py-4 flex items-center justify-between">
          <h2 class="text-xl font-bold text-gray-800">{{ curChat.name }}</h2>
          <button class="text-gray-600 hover:text-gray-800" @click="openSettingsPanel">
            <SettingsIcon class="h-6 w-6" />
          </button>
        </div>

        <!-- 聊天消息区域 -->
        <div v-if='curChat.message_type == "private" || curChat.message_type == "group"'
          class="flex-1 overflow-y-auto px-6 py-4 h-[calc(100%-120px)]" ref="messageContainer">
          <div v-for="(message, index) in messages" :key="index" class="mb-4">
            <div :class="[
              'max-w-xs p-3 rounded-lg',
              message.sender_id == user.id ? 'bg-indigo-100 ml-auto' : 'bg-gray-200'
            ]">
              <p class="text-sm">{{ message.content }}</p>
            </div>
            <p :class="[
              'text-xs mt-1 text-gray-500',
              message.sender_id == user.id ? 'text-right' : 'text-left'
            ]">
              {{ message.sent_at }}
            </p>
          </div>
          <div ref="bottomEl"></div>
        </div>
        <div v-else-if="curChat.message_type == 'notice'" class="flex-1 overflow-y-auto px-6 py-4 h-[calc(100%-120px)]">
          <div v-for="(message,idx) in notices" :key="idx" class="mb-4">
            <div v-if="message.type === 'friend'" class="max-w-xs p-3 rounded-lg">
              <p class="text-lg">{{ message.username }} 请求添加你为好友</p>
              <div class="flex space-x-4 mt-2">
                <!-- Accept Button -->
                <button @click="acceptFriendRequest(message.user_id,idx)"
                  class="px-4 py-2 bg-green-500 text-white rounded-md shadow hover:bg-green-600 focus:outline-none focus:ring-2 focus:ring-green-400 focus:ring-offset-2">
                  Accept
                </button>

                <!-- Decline Button 无操作 -->
                <button
                  class="px-4 py-2 bg-red-500 text-white rounded-md shadow hover:bg-red-600 focus:outline-none focus:ring-2 focus:ring-red-400 focus:ring-offset-2">
                  Decline
                </button>
              </div>

            </div>
            <div v-else-if="message.type== 'admin_group'" class="max-w-xs p-3 rounded-lg">
              <p class="text-lg">{{ message.user_id }}请求加入群</p>
              <div class="flex space-x-4 mt-2">
                <!-- Accept Button -->
                <button @click="acceptGroupRequest(message.user_id,message.group_id,idx)"
                  class="px-4 py-2 bg-green-500 text-white rounded-md shadow hover:bg-green-600 focus:outline-none focus:ring-2 focus:ring-green-400 focus:ring-offset-2">
                  Accept
                </button>

                <!-- Decline Button 无操作 -->
                <button
                  class="px-4 py-2 bg-red-500 text-white rounded-md shadow hover:bg-red-600 focus:outline-none focus:ring-2 focus:ring-red-400 focus:ring-offset-2">
                  Decline
                </button>
              </div>

            </div>
          </div>
        </div>
        <!-- 输入框区域 -->
        <div class="bg-white px-6 py-4 border-t">
          <form @submit.prevent="sendMessage" class="flex items-center">
            <input v-model="newMessage" type="text" placeholder="输入消息..."
              class="flex-grow px-4 py-2 bg-gray-100 border border-gray-300 rounded-l-md focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:border-transparent" />
            <button type="submit"
              class="bg-indigo-600 text-white px-4 py-2 rounded-r-md hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-indigo-500 focus:ring-offset-2">
              <SendIcon class="h-5 w-5" />
            </button>
          </form>
        </div>
      </div>
      <div v-else class="flex-1 flex flex-col bg-white">
        <div class="bg-white shadow px-6 py-4 flex items-center justify-between">
          <h2 class="text-xl font-bold text-gray-800">搜索</h2>
          <button @click="closeSearchPanel" class="text-gray-600 hover:text-gray-800">
            <XIcon class="h-6 w-6" />
          </button>
        </div>
        <div class="p-4">
          <div class="relative">
            <input v-model="searchQuery" @keydown.enter="handleSearch" type="text" placeholder="搜索用户或群组..."
              class="w-full px-4 py-2 bg-gray-100 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-indigo-500" />
            <div @click="handleSearch"
              class="absolute right-3 top-[5px] h-8 w-8 text-gray-400 hover:bg-gray-500 rounded cursor-pointer items-center justify-center flex ">
              <SearchIcon class="h-5 w-5 text-gray-400" />
            </div>
          </div>
        </div>
        <div class="flex-1 overflow-y-auto px-4">
          <div class="p-3 rounded-md">用户</div>
          <div v-for="result in searchResultsFriend" :key="result.id"
            class="p-3 hover:bg-gray-100 cursor-pointer rounded-md" @click="()=>{
              currentAction = 'addFriend'
              modalInput = `${result.id}`
              // handleAddAction()
              openAddActionModal('addFriend')
            }">
            <div class="flex items-center space-x-3">
              <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center text-white">
                <UserIcon class="h-6 w-6" />
              </div>
              <div class="flex-1">
                <p class="font-semibold text-gray-800">{{ result.username }}</p>
                <!-- <p class="text-xs text-gray-500">{{ result.type === 'user' ? '用户' : '群组' }}</p> -->
              </div>
            </div>
          </div>
          <div class="p-3 rounded-md">群组</div>
          <div v-for="result in searchResultsGroup" :key="result.id"
            class="p-3 hover:bg-gray-100 cursor-pointer rounded-md" @click="()=>{
              currentAction = 'createGroup'
              // modalInput = result.group_name
              modalInput = `${result.id}`
              openAddActionModal('addGroup')
            }">
            <div class="flex items-center space-x-3">
              <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center text-white">
                <UsersIcon class="h-6 w-6" />
              </div>
              <div class="flex-1">
                <p class="font-semibold text-gray-800">{{ result.group_name }}</p>
                <!-- <p class="text-xs text-gray-500">{{ result.type === 'user' ? '用户' : '群组' }}</p> -->
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
    <!-- 滚动式通知列表 -->
    <div class="fixed top-4 right-4 w-80 max-h-[calc(100vh-2rem)] overflow-y-auto space-y-2 z-50">
      <transition-group name="notification">
        <div v-for="notif in notifications" :key="notif.id" class="bg-white shadow-lg rounded-lg p-4">
          <div class="flex items-center">
            <div class="flex-shrink-0">
              <InfoIcon class="h-5 w-5 text-indigo-500" />
            </div>
            <div class="ml-3 w-0 flex-1 pt-0.5">
              <p class="text-sm font-medium text-gray-900">{{ notif.title }}</p>
              <p class="mt-1 text-sm text-gray-500">{{ notif.message }}</p>
            </div>
            <div class="ml-4 flex-shrink-0 flex">
              <button @click="removeNotification(notif.id)"
                class="bg-white rounded-md inline-flex text-gray-400 hover:text-gray-500 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500">
                <span class="sr-only">关闭</span>
                <XIcon class="h-5 w-5" />
              </button>
            </div>
          </div>
        </div>
      </transition-group>
    </div>
  </div>

  <!-- 设置面板 -->
  <div v-if="isSettingsPanelOpen" class="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
    <div class="bg-white rounded-lg p-6 w-96 max-h-[80vh] overflow-y-auto">
      <div class="flex justify-between items-center mb-4">
        <h3 class="text-lg font-semibold">设置</h3>
        <button @click="closeSettingsPanel" class="text-gray-500 hover:text-gray-700">
          <XIcon class="h-6 w-6" />
        </button>
      </div>
      <div class="space-y-4">
        <div v-if="curChat">
          <h4 class="font-medium mb-2">{{ curChat.message_type === 'private' ? '私聊设置' : '群聊设置' }}</h4>
          <div v-if="curChat.message_type === 'private'" class="space-y-2">
            <button @click="deleteFriend"
              class="w-full py-2 px-4 bg-red-600 text-white rounded hover:bg-red-700 transition duration-200">
              删除好友
            </button>
          </div>
          <div v-else-if="curChat.message_type === 'group'" class="space-y-2">
            <button v-if="(curChat.user_role == 'admin' || curChat.user_role == 'owner')" @click="openMemberManagement"
              class="w-full py-2 px-4 bg-indigo-600 text-white rounded hover:bg-indigo-700 transition duration-200">
              管理成员
            </button>
            <button v-if="(curChat.user_role != 'owner')" @click="exitGroup"
              class="w-full py-2 px-4 bg-red-600 text-white rounded hover:bg-red-700 transition duration-200">
              退出群组
            </button>
            <button v-else @click="deleteGroup"
              class="w-full py-2 px-4 bg-red-600 text-white rounded hover:bg-red-700 transition duration-200">
              解散群组
            </button>
          </div>
        </div>
      </div>
    </div>
  </div>

  <!-- 添加好友/群聊弹窗 -->
  <div v-if="isAddActionModalOpen" class="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
    <div class="bg-white rounded-lg p-6 w-80">
      <h3 class="text-lg font-semibold mb-4">{{ modalTitle }}</h3>
      <input v-model="modalInput" type="text" :placeholder="modalPlaceholder"
        class="w-full px-3 py-2 border border-gray-300 rounded-md mb-4 focus:outline-none focus:ring-2 focus:ring-indigo-500" />
      <div class="space-y-3">
        <button @click="handleAddAction"
          class="w-full py-2 px-4 bg-indigo-600 text-white rounded hover:bg-indigo-700 transition duration-200">
          {{ modalActionText }}
        </button>
        <button @click="closeAddActionModal"
          class="w-full py-2 px-4 bg-gray-200 text-gray-800 rounded hover:bg-gray-300 transition duration-200">
          取消
        </button>
      </div>
    </div>
  </div>

  <!-- 成员管理模态框 -->
  <div v-if="isMemberManagementOpen" class="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
    <div class="bg-white rounded-lg p-6 w-96 max-h-[80vh] overflow-y-auto">
      <div class="flex justify-between items-center mb-4">
        <h3 class="text-lg font-semibold">成员管理</h3>
        <button @click="closeMemberManagement" class="text-gray-500 hover:text-gray-700">
          <XIcon class="h-6 w-6" />
        </button>
      </div>
      <div class="space-y-4">
        <div v-for="member in members" :key="member.id" class="flex items-center justify-between">
          <div class="flex items-center space-x-3">
            <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center text-white">
              <UserIcon class="h-6 w-6" />
            </div>
            <div>
              <p class="font-semibold">{{ member.nickname }}</p>
              <p class="text-xs text-gray-500">{{ member.role }}</p>
            </div>
          </div>
          <div class="space-x-2">
            <button v-if="member.role!='owner'" @click="changeMemberRole(member)"
              class="px-2 py-1 bg-indigo-100 text-indigo-700 rounded hover:bg-indigo-200 transition duration-200">
              更改权限
            </button>
            <button v-if="member.role!='owner'" @click="removeMember(member)"
              class="px-2 py-1 bg-red-100 text-red-700 rounded hover:bg-red-200 transition duration-200">
              踢出群组
            </button>
          </div>
        </div>
      </div>
    </div>
  </div>
  <div v-if="isUserProfilePanelOpen" class="fixed inset-0 bg-black bg-opacity-50 flex items-center justify-center z-50">
    <div class="bg-white rounded-lg p-6 w-96">
      <div class="flex justify-between items-center mb-4">
        <h3 class="text-lg font-semibold">编辑个人信息</h3>
        <button @click="closeUserProfilePanel" class="text-gray-500 hover:text-gray-700">
          <XIcon class="h-6 w-6" />
        </button>
      </div>
      <form @submit.prevent="updateUserProfile" class="space-y-4">
        <div>
          <label for="name" class="block text-sm font-medium text-gray-700">名字</label>
          <input id="name" v-model="editingUserProfile.username" type="text"
            class="mt-1 block w-full px-3 py-2 bg-white border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500" />
        </div>
        <div>
          <label for="birthdate" class="block text-sm font-medium text-gray-700">出生日期</label>
          <input id="birthdate" v-model="editingUserProfile.birthdate" type="date"
            class="mt-1 block w-full px-3 py-2 bg-white border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500" />
        </div>
        <div>
          <label for="region" class="block text-sm font-medium text-gray-700">地区</label>
          <input id="region" v-model="editingUserProfile.location" type="text"
            class="mt-1 block w-full px-3 py-2 bg-white border border-gray-300 rounded-md shadow-sm focus:outline-none focus:ring-indigo-500 focus:border-indigo-500" />
        </div>
        <div class="flex justify-end space-x-3">
          <button type="button" @click="closeUserProfilePanel"
            class="px-4 py-2 border border-gray-300 rounded-md shadow-sm text-sm font-medium text-gray-700 bg-white hover:bg-gray-50 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500">
            取消
          </button>
          <button type="submit"
            class="px-4 py-2 border border-transparent rounded-md shadow-sm text-sm font-medium text-white bg-indigo-600 hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500">
            保存
          </button>
          <button type="button" @click="openBindPage"
            class="px-4 py-2 border border-transparent rounded-md shadow-sm text-sm font-medium text-white bg-indigo-600 hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500">
            绑定微信
          </button>
        </div>
      </form>
    </div>
  </div>
</template>

<script lang="ts" setup>
// eslint-disable-next-line @typescript-eslint/ban-ts-comment
//@ts-nocheck
import { ref,onMounted, watch,onUnmounted } from 'vue'
import { UsersIcon, UserIcon, SettingsIcon,InfoIcon ,SendIcon, UserPlusIcon, CircleAlertIcon,SearchIcon,XIcon, LogOutIcon } from 'lucide-vue-next'
import { wx } from '@/stores/axios'
import { useRouter } from 'vue-router'

const isUserProfilePanelOpen = ref(false)
const isSettingsPanelOpen = ref(false)
const isMemberManagementOpen = ref(false)
const isAddActionModalOpen = ref(false)
const isSearchPanelOpen = ref(false)
const activeTab = ref('messages')
const friends = ref([])
const groups = ref([])
const user = ref({});
const chats = ref([])
const modalTitle = ref('')
const modalPlaceholder = ref('')
const modalActionText = ref('')
const modalInput = ref('')
const currentAction = ref('')
const bottomEl = ref(null)
const editingUserProfile = ref({ ...user.value })
const searchQuery = ref('')
const searchResultsFriend = ref([])
const searchResultsGroup = ref([])
const notifications = ref([])
const messages = ref([])
const members = ref([])
const notices = ref([])
const newMessage = ref('')
const curChat = ref({
  user_id: null,
  group_id: null,
  message_type: 'private',
  name: ''
})
const router = useRouter();

const openSearchPanel = () => {
  isSearchPanelOpen.value = true
}
const acceptFriendRequest = (id,idx) => {
  console.log('Accepting friend request')
  // console.log(id)
  wx("verifyFriend",{friend_id: id}).then(res=>{
    if(res.code == 200){
      console.log(res.data)
      // notices.slice(idx,1)
      notices.value.splice(idx,1)
    }
  })
}
const closeSearchPanel = () => {
  isSearchPanelOpen.value = false
  searchQuery.value = ''
}
const handleSearch = () => {
  if (searchQuery.value.trim()) {
    // 实现搜索逻辑
    console.log('Searching for:', searchQuery.value)
    wx("searchFriend",{username: searchQuery.value}).then(res=>{
      if(res.code == 200){
        console.log(res.data)
        searchResultsFriend.value = res.data.result;
      }
    });
    wx("searchGroup",{group_name: searchQuery.value}).then(res=>{
      if(res.code == 200){
        console.log(res.data)
        searchResultsGroup.value = res.data.result;
      }
    })
  }
}
const scrollToBottom = () => {
  if (bottomEl.value) {
    console.log('scrolling')
    bottomEl.value.scrollIntoView();
  }
};
const acceptGroupRequest = (user_id,group_id,idx) => {
  console.log('Accepting group request')
  wx("verifyGroup",{group_id: group_id,user_id: user_id}).then(res=>{
    if(res.code == 200){
      console.log(res.data)
      notices.value.splice(idx,1)
    }
  })
}

const openAddActionModal = (action) => {
  currentAction.value = action
  if (action === 'addFriend') {
    modalTitle.value = '添加好友'
    modalPlaceholder.value = '请输入好友的用户名或ID'
    modalActionText.value = '添加好友'
  } else if (action === 'createGroup') {
    modalTitle.value = '创建群组'
    modalPlaceholder.value = '请输入群组名称'
    modalActionText.value = '创建群组'
  } else if(action === "addGroup") {
    modalTitle.value = '加入群组'
    modalPlaceholder.value = '请输入群组ID'
    modalActionText.value = '添加群组'
  }
  // modalInput.value = ''
  isAddActionModalOpen.value = true
}
const closeAddActionModal = () => {
  isAddActionModalOpen.value = false
}

const handleAddAction = () => {
  console.log(modalInput.value)
  if (modalInput.value.trim()) {
    if (currentAction.value === 'addFriend') {
      // console.log('Adding friend:', modalInput.value)
      wx("addFriend",{friend_id: parseInt(modalInput.value)}).then(res=>{
        if(res.code == 200){
          friends.value.push(res.data)
        }
      })
    } else if (currentAction.value === 'createGroup') {
      // console.log('Creating group:', modalInput.value)
      wx("createGroup",{group_name: modalInput.value}).then(res=>{
        if(res.code == 200){
          groups.value.push(res.data)
        }
      })
      // 实现创建群组的逻辑
    } else if(currentAction.value === 'addGroup') {
      wx("addGroup",{group_id: parseInt(modalInput.value)}).then(res=>{
        if(res.code == 200){
          groups.value.push(res.data)
        }
      })
    }
    closeAddActionModal()
  }
}
const sendMessage = () => {
  if (newMessage.value.trim()) {
    if(curChat.value.message_type == "private") {
      const friend_id = curChat.value.sender_id == user.value.id ? curChat.value.receiver_id : curChat.value.sender_id;
      if(friend_id == null){
        friend_id = curChat.value.user_id;
      }
      wx("sendMessage",{message: newMessage.value,friend_id: friend_id}).then(res=>{
      if(res.code == 200){
        newMessage.value = '';
        getMessage();
      }
      if(res.code == 404){
        showNotification("发送消息失败","对方不是你的好友")
        newMessage.value = '';
      }
    })
    }else{
      wx("sendGroupMessage",{message: newMessage.value,group_id: curChat.value.group_id}).then(res=>{
      if(res.code == 200){
        newMessage.value = '';
        getMessage();
      }
      if(res.code == 404){
        showNotification("发送消息失败","你不在该群组中")
        newMessage.value = '';
      }
    })
    }
  }
}
const markAsRead = (data) => {
  // console.log(data);
  if(data.sender_id != user.value.id){
    showNotification("新消息",data.content);
  }
  wx("markRead",{message_id: data.message_id}).then(res=>{
    if(res.code == 200){
      // console.log(res.data)
    }});
}
const getInfo = () => {
  wx("getInfo").then(res=>{
    if(res.code == 200){
      user.value = res.data.user;
      friends.value = res.data.friends
      groups.value = res.data.groups;
    }else{
      router.push('/wx/login')
    }
  })
  wx("getMessage").then(res=>{
    if(res.code == 200){
      // console.log(res.data)
      res.data.message.map(markAsRead)
      notices.value = res.data.notice;
      res.data.notice.map((data)=>{
        showNotification("新通知",`${data.username}请求${data.type == 'friend' ? '添加你为好友' : '加入群组'}`)
      })
    }
  })
}
const getMessage = () => {
  if(curChat.value.message_type == "private"){
      const friend_id =  curChat.value.sender_id == user.value.id ? curChat.value.receiver_id : curChat.value.sender_id;
      wx("getHistoryMessage",{friend_id: friend_id}).then(res=>{
        if(res.code == 200){
          messages.value = res.data;
        }
      })
    }else if(curChat.value.message_type == "group"){
      wx("getHistoryMessage",{group_id: curChat.value.group_id}).then(res=>{
        if(res.code == 200){
          messages.value = res.data
        }
      })
    }
    messages.value.sort((a,b)=>{
      return - new Date(a.sent_at) + new Date(b.sent_at)
    })
}
const getRecentMessage = () => {
  wx("getRecentMessage").then(res=>{
    if(res.code == 200){
      if(res.data.length > 0){
        if(curChat.value.name == ""){
          curChat.value = res.data[0];
        }
        chats.value = res.data;
      }
    }
  });
}
const openSettingsPanel = () => {
  isSettingsPanelOpen.value = true
}
const closeSettingsPanel = () => {
  isSettingsPanelOpen.value = false
}
const deleteFriend = () => {
  console.log(curChat.value)
  if (curChat.value && curChat.value.message_type === 'private') {
    const friend_id = curChat.value.sender_id == user.value.id ? curChat.value.receiver_id : curChat.value.sender_id;
    wx("deleteFriend",{friend_id: friend_id}).then(res=>{
      if(res.code == 200){
        friends.value = friends.value.filter(friend => friend.id !== friend_id)
      }
    })
    showNotification('删除好友', `已成功删除好友：${curChat.value.name}`)
    closeSettingsPanel()
  }
}
const openMemberManagement = () => {
  console.log(curChat.value)
  if (curChat.value && curChat.value.message_type === 'group' && (curChat.value.user_role === 'admin' || curChat.value.user_role === 'owner')) {
    wx("getGroup",{group_id: curChat.value.group_id}).then(res=>{
      if(res.code == 200){
        members.value = res.data
        isMemberManagementOpen.value = true
      }else{
        showNotification('获取群组信息失败', '请稍后再试')
      }
    })
  }
}
const closeMemberManagement = () => {
  isMemberManagementOpen.value = false
}
const exitGroup = () => {
  if (curChat.value && curChat.value.message_type === 'group' && curChat.value.user_role !== 'owner') {
    wx("exitGroup",{group_id: curChat.value.group_id}).then(res=>{
      if(res.code == 200){
        groups.value = groups.value.filter(group => group.id !== curChat.value.group_id)
      }
    })
    showNotification('退出群组', `已成功退出群组：${curChat.value.name}`)
    closeSettingsPanel()
  }
}
const deleteGroup = () => {
  if (curChat.value && curChat.value.message_type === 'group' && curChat.value.user_role === 'owner') {
    wx("deleteGroup",{group_id: curChat.value.group_id}).then(res=>{
      if(res.code == 200){
        groups.value = groups.value.filter(group => group.id !== curChat.value.group_id)
      }
    })
    showNotification('解散群组', `已成功解散群组：${curChat.value.name}`)
    closeSettingsPanel()
  }
}
const changeMemberRole = (member) => {
  // console.log('Changing role for member:', member.name)
  wx("modifyPermission",{group_id: curChat.value.group_id,user_id: member.id, role: member.role == 'member' ? 'admin' : 'member'
  }).then(res=>{
    if(res.code == 200){
      members.value = members.value.map(m => {
        if (m.id === member.id) {
          m.role = member.role == 'member' ? 'admin' : 'member'
        }
        return m
      })
      showNotification('更改权限', `已更改 ${member.nickname} 的权限为 ${member.role == 'member' ? '管理员' : '成员'}`)
    }
  })
}
const removeMember = (member) => { // TODO:
  wx("kickGroupmember",{group_id: curChat.value.group_id,user_id: member.id}).then(res=>{
    if(res.code == 200){
      members.value = members.value.filter(m => m.id !== member.id)
      showNotification('移除成员', `已将 ${member.nickname} 移出群组`)
    }
  })
}

watch(messages,()=>{
  scrollToBottom();
})
onMounted(()=>{
  getInfo();
  getMessage();
  getRecentMessage();
  const interval = setInterval(()=>{
    getMessage();
    getInfo();
    getRecentMessage();
  },1000)
  onUnmounted(()=>{
    clearInterval(interval)
    // clearInterval(r)
  })
})

let notificationId = 0
const showNotification = (title, message) => {
  const id = notificationId++
  notifications.value.push({ id, title, message })
  setTimeout(() => {
    removeNotification(id)
  }, 5000) // 5秒后自动移除通知
}

const removeNotification = (id) => {
  const index = notifications.value.findIndex(notif => notif.id === id)
  if (index !== -1) {
    notifications.value.splice(index, 1)
  }
}
const openUserProfilePanel = () => {
  console.log(user.value)
  editingUserProfile.value = { ...user.value }
  isUserProfilePanelOpen.value = true
}

const closeUserProfilePanel = () => {
  isUserProfilePanelOpen.value = false
}

const updateUserProfile = () => {
  user.value = { ...editingUserProfile.value }
  wx("updateProfile",editingUserProfile.value).then(res=>{
    if(res.code == 200){
      showNotification('更新个人信息', '您的个人信息已成功更新')
    }
  })
  closeUserProfilePanel()
}
</script>
<style scoped>
.notification-enter-active,
.notification-leave-active {
  transition: all 0.5s ease;
}

.notification-enter-from,
.notification-leave-to {
  opacity: 0;
}
</style>
