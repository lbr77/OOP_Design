<template>
  <div class="min-h-screen bg-gray-100 flex items-center justify-center p-4">
    <div
      class="max-w-6xl w-full max-h-[800px] min-h-[600px] min-w-[320px] h-full bg-white shadow-xl rounded-lg flex overflow-hidden">
      <!-- 侧边栏 -->
      <div class="w-1/4 min-w-[200px] max-w-[300px] bg-indigo-800 text-white flex flex-col">
        <!-- 用户信息和操作按钮 -->
        <div class="p-4 flex items-center justify-between">
          <div class="flex items-center space-x-3">
            <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
              <UserIcon class="h-6 w-6" />
            </div>
            <div>
              <p class="font-semibold">用户名</p>
              <p class="text-xs text-indigo-300">在线</p>
            </div>
          </div>
          <div class="flex space-x-2">
            <button @click="openSearchPanel" class="text-white hover:text-indigo-200" aria-label="搜索">
              <SearchIcon class="h-5 w-5" />
            </button>
            <button @click="openAddActionModal('addFriend')" class="text-white hover:text-indigo-200" aria-label="添加好友">
              <UserPlusIcon class="h-5 w-5" />
            </button>
            <button @click="openAddActionModal('createGroup')" class="text-white hover:text-indigo-200"
              aria-label="创建群组">
              <UsersIcon class="h-5 w-5" />
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
        </div>

        <!-- 消息/好友列表 -->
        <div class="flex-1 overflow-y-auto">
          <!-- 消息列表 -->
          <div v-if="activeTab === 'messages'" class="h-full">
            <div v-for="chat in chats" :key="chat.id" class="p-3 hover:bg-indigo-700 cursor-pointer">
              <div class="flex items-center space-x-3">
                <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
                  <UserIcon class="h-6 w-6" />
                </div>
                <div class="flex-1">
                  <p class="font-semibold">{{ chat.name }}</p>
                  <p class="text-xs text-indigo-300 truncate">{{ chat.lastMessage }}</p>
                </div>
              </div>
            </div>
          </div>

          <!-- 好友列表 -->
          <div v-else class="h-full">
            <div v-for="friend in friends" :key="friend.id" class="p-3 hover:bg-indigo-700 cursor-pointer">
              <div class="flex items-center space-x-3">
                <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
                  <UserIcon class="h-6 w-6" />
                </div>
                <div>
                  <p class="font-semibold">{{ friend.name }}</p>
                  <p class="text-xs text-indigo-300">{{ friend.status }}</p>
                </div>
              </div>
            </div>
          </div>
        </div>
      </div>

      <!-- 聊天区域或搜索面板 -->
      <div v-if="!isSearchPanelOpen" class="flex-1 flex flex-col">
        <!-- 聊天框头部 -->
        <div class="bg-white shadow px-6 py-4 flex items-center justify-between">
          <h2 class="text-xl font-bold text-gray-800">聊天室</h2>
          <button class="text-gray-600 hover:text-gray-800">
            <SettingsIcon class="h-6 w-6" />
          </button>
        </div>

        <!-- 聊天消息区域 -->
        <div class="flex-1 overflow-y-auto px-6 py-4 h-[calc(100%-120px)]">
          <div v-for="(message, index) in messages" :key="index" class="mb-4">
            <div :class="[
              'max-w-xs p-3 rounded-lg',
              message.isSelf ? 'bg-indigo-100 ml-auto' : 'bg-gray-200'
            ]">
              <p class="text-sm">{{ message.text }}</p>
            </div>
            <p :class="[
              'text-xs mt-1 text-gray-500',
              message.isSelf ? 'text-right' : 'text-left'
            ]">
              {{ message.time }}
            </p>
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

      <!-- 搜索面板 -->
      <div v-else class="flex-1 flex flex-col bg-white">
        <div class="bg-white shadow px-6 py-4 flex items-center justify-between">
          <h2 class="text-xl font-bold text-gray-800">搜索</h2>
          <button @click="closeSearchPanel" class="text-gray-600 hover:text-gray-800">
            <XIcon class="h-6 w-6" />
          </button>
        </div>
        <div class="p-4">
          <div class="relative">
            <input v-model="searchQuery" @input="handleSearch" type="text" placeholder="搜索用户或群组..."
              class="w-full px-4 py-2 bg-gray-100 border border-gray-300 rounded-md focus:outline-none focus:ring-2 focus:ring-indigo-500" />
            <SearchIcon class="absolute right-3 top-2.5 h-5 w-5 text-gray-400" />
          </div>
        </div>
        <div class="flex-1 overflow-y-auto px-4">
          <div v-for="result in searchResults" :key="result.id" class="p-3 hover:bg-gray-100 cursor-pointer rounded-md">
            <div class="flex items-center space-x-3">
              <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center text-white">
                <component :is="result.type === 'user' ? UserIcon : UsersIcon" class="h-6 w-6" />
              </div>
              <div class="flex-1">
                <p class="font-semibold text-gray-800">{{ result.name }}</p>
                <p class="text-xs text-gray-500">{{ result.type === 'user' ? '用户' : '群组' }}</p>
              </div>
            </div>
          </div>
        </div>
      </div>
    </div>
  </div>

  <!-- 添加好友/创建群组模态框 -->
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
</template>

<script setup>
import { ref, computed } from 'vue'
import { UserIcon, SettingsIcon, SendIcon, UserPlusIcon, UsersIcon, SearchIcon, XIcon } from 'lucide-vue-next'

const activeTab = ref('messages')
const isAddActionModalOpen = ref(false)
const isSearchPanelOpen = ref(false)
const modalTitle = ref('')
const modalPlaceholder = ref('')
const modalActionText = ref('')
const modalInput = ref('')
const currentAction = ref('')
const searchQuery = ref('')

const chats = ref([
  { id: 1, name: "Alice", lastMessage: "你好，最近怎么样？" },
  { id: 2, name: "Bob", lastMessage: "周末有空吗？" },
  { id: 3, name: "Charlie", lastMessage: "项目进展如何？" },
])

const friends = ref([
  { id: 1, name: "Alice", status: "在线" },
  { id: 2, name: "Bob", status: "离线" },
  { id: 3, name: "Charlie", status: "忙碌" },
])

const groups = ref([
  { id: 1, name: "前端开发群", type: 'group' },
  { id: 2, name: "产品讨论群", type: 'group' },
])

const messages = ref([
  { text: "你好！欢迎来到聊天室。", time: "10:00", isSelf: false },
  { text: "谢谢！很高兴加入。", time: "10:02", isSelf: true },
  { text: "有什么我可以帮助你的吗？", time: "10:03", isSelf: false },
  { text: "我想了解更多关于Vue 3的信息。", time: "10:05", isSelf: true },
])

const newMessage = ref('')

const sendMessage = () => {
  if (newMessage.value.trim()) {
    messages.value.push({
      text: newMessage.value,
      time: new Date().toLocaleTimeString([], { hour: '2-digit', minute: '2-digit' }),
      isSelf: true
    })
    newMessage.value = ''
    // 这里可以添加发送消息到服务器的逻辑
  }
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
  }
  modalInput.value = ''
  isAddActionModalOpen.value = true
}

const closeAddActionModal = () => {
  isAddActionModalOpen.value = false
}

const handleAddAction = () => {
  if (modalInput.value.trim()) {
    if (currentAction.value === 'addFriend') {
      console.log('Adding friend:', modalInput.value)
      // 实现添加好友的逻辑
    } else if (currentAction.value === 'createGroup') {
      console.log('Creating group:', modalInput.value)
      // 实现创建群组的逻辑
    }
    closeAddActionModal()
  }
}

const openSearchPanel = () => {
  isSearchPanelOpen.value = true
}

const closeSearchPanel = () => {
  isSearchPanelOpen.value = false
  searchQuery.value = ''
}

const searchResults = computed(() => {
  if (!searchQuery.value) return []

  const query = searchQuery.value.toLowerCase()
  const userResults = friends.value
    .filter(friend => friend.name.toLowerCase().includes(query))
    .map(friend => ({ ...friend, type: 'user' }))

  const groupResults = groups.value
    .filter(group => group.name.toLowerCase().includes(query))

  return [...userResults, ...groupResults]
})

const handleSearch = () => {
  // 这里可以添加额外的搜索逻辑，比如从服务器获取搜索结果
  console.log('Searching for:', searchQuery.value)
}
</script>
