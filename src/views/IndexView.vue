<template>
  <div class="min-h-screen bg-gray-100 flex items-center justify-center p-4">
    <div
      class="max-w-6xl w-full max-h-[800px] min-h-[800px] min-w-[320px] h-full bg-white shadow-xl rounded-lg flex overflow-hidden">
      <!-- 侧边栏 -->
      <div class="w-1/4 min-w-[200px] max-w-[300px] bg-indigo-800 text-white">
        <!-- 用户信息 -->
        <div class="relative p-4 flex items-center space-x-3">
          <div class="w-10 h-10 rounded-full bg-indigo-600 flex items-center justify-center">
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
          <div v-for="friend in friends" :key="friend.id" class="p-3 hover:bg-indigo-700 cursor-pointer"
            @click="curChat.user_id = friend.id">
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
        <div v-if="activeTab === 'group'" class="overflow-y-auto h-[calc(100%-120px)] min-h-[400px]">
          <div v-for="group in groups" :key="group.id" class="p-3 hover:bg-indigo-700 cursor-pointer"
            @click="curChat.group_id = group.id">
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
          <button class="text-gray-600 hover:text-gray-800">
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
                <button @click=""
                  class="px-4 py-2 bg-green-500 text-white rounded-md shadow hover:bg-green-600 focus:outline-none focus:ring-2 focus:ring-green-400 focus:ring-offset-2">
                  Accept
                </button>

                <!-- Decline Button -->
                <button @click=""
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
  </div>
</template>

<script setup>
import { ref,onMounted, watch,onUnmounted } from 'vue'
import { UsersIcon, UserIcon, SettingsIcon, SendIcon, UserPlusIcon, CircleAlertIcon,SearchIcon,XIcon } from 'lucide-vue-next'
import { qq } from '@/stores/axios'
import { useRouter } from 'vue-router'
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
const curChat = ref({
  user_id: null,
  group_id: null
})
const openSearchPanel = () => {
  isSearchPanelOpen.value = true
}

const closeSearchPanel = () => {
  isSearchPanelOpen.value = false
  searchQuery.value = ''
}
const scrollToBottom = () => {
  if (bottomEl) {
    console.log('scrolling')
    bottomEl.value.scrollIntoView({ behavior: 'smooth' })
  }
};


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
      // console.log('Adding friend:', modalInput.value)
      qq("addFriend",{friend_id: modalInput.value}).then(res=>{
        if(res.code == 200){
          friends.value.push(res.data)
        }
      })
    } else if (currentAction.value === 'createGroup') {
      // console.log('Creating group:', modalInput.value)
      qq("createGroup",{group_name: modalInput.value}).then(res=>{
        if(res.code == 200){
          groups.value.push(res.data)
        }
      })
      // 实现创建群组的逻辑
    }
    closeAddActionModal()
  }
}
const messages = ref([])
const notices = ref([])
const newMessage = ref('')
watch(messages,()=>{
  scrollToBottom();
})
const sendMessage = () => {
  if (newMessage.value.trim()) {
    if(curChat.value.message_type == "private") {
      let friend_id = curChat.value.sender_id == user.value.id ? curChat.value.receiver_id : curChat.value.sender_id;
      qq("sendMessage",{message: newMessage.value,friend_id: friend_id}).then(res=>{
      if(res.code == 200){
        newMessage.value = '';
        getMessage();
      }
    })
    }else{
      qq("sendGroupMessage",{message: newMessage.value,group_id: curChat.value.group_id}).then(res=>{
      if(res.code == 200){
        newMessage.value = '';
        getMessage();
      }
    })
    }
  }
}
const router = useRouter();
const getMessage = () => {
  if(curChat.value.message_type == "private"){
      let friend_id =  curChat.value.sender_id == user.value.id ? curChat.value.receiver_id : curChat.value.sender_id;
      qq("getHistoryMessage",{friend_id: friend_id}).then(res=>{
        if(res.code == 200){
          messages.value = res.data;
        }
      })
    }else if(curChat.value.message_type == "group"){
      qq("getHistoryMessage",{group_id: curChat.value.group_id}).then(res=>{
        if(res.code == 200){
          messages.value = res.data
        }
      })
    }
    messages.value.sort((a,b)=>{
      return - new Date(a.sent_at) + new Date(b.sent_at)
    })
}
watch(curChat,()=>{
  getMessage();
});

onMounted(()=>{
  qq("getInfo").then(res=>{
    if(res.code == 200){
      user.value = res.data.user;
      friends.value = res.data.friends
      groups.value = res.data.groups;
    }else{
      router.push('/login')
    }
  })
  qq("getMessage").then(res=>{
    if(res.code == 200 ){
      messages.value = res.data.message;
      notices.value = res.data.notice;
      if(res.data.message.length > 0){
        if(res.data.message[0].message_type == "group"){
          curChat.value.group_id = res.data.message[0].group_id;
        }else{
          curChat.value.user_id = res.data.message[0].sender_id == user.value.id ? res.data.message[0].receiver_id : res.data.message[0].sender_id;
        }
      }

    }
  })
  qq("getRecentMessage").then(res=>{
    if(res.code == 200){
      // console.log(res)
      curChat.value = res.data[0];
      chats.value = res.data;
    }
  });
  let interval = setInterval(()=>{
    getMessage();
  },1000)
  onUnmounted(()=>{
    clearInterval(interval)
  })
})
</script>
