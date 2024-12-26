<template>
  <div class="min-h-screen bg-gray-100 flex items-center justify-center py-12 px-4 sm:px-6 lg:px-8">
    <div class="max-w-md w-full space-y-8">
      <div>
        <h2 class="mt-6 text-center text-3xl font-extrabold text-gray-900">QQ - 注册</h2>
      </div>
      <form class="mt-8 space-y-6" @submit.prevent="handleSubmit">
        <div class="rounded-md shadow-sm -space-y-px">
          <div>
            <label for="username" class="sr-only">用户名</label>
            <!-- <input id="email-address" name="email" type="email" autocomplete="email" required v-model="email"
              class="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-t-md focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm"
              placeholder="邮箱地址" /> -->
            <input id="username" name="username" type="text" autocomplete="username" required v-model="username"
              class="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-t-md focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm"
              placeholder="用户名" />
          </div>
          <div>
            <label for="password" class="sr-only">密码</label>
            <input id="password" name="password" type="password" autocomplete="current-password" required
              v-model="password"
              class="appearance-none rounded-none relative block w-full px-3 py-2 border border-gray-300 placeholder-gray-500 text-gray-900 rounded-b-md focus:outline-none focus:ring-indigo-500 focus:border-indigo-500 focus:z-10 sm:text-sm"
              placeholder="密码" />
          </div>
        </div>

        <div class="flex items-center justify-between">

          <div class="text-sm">
            <RouterLink to="/wx/login" class="font-medium text-indigo-600 hover:text-indigo-500">
              已有账户？登录
            </RouterLink>
          </div>
        </div>

        <div>
          <button type="submit"
            class="group relative w-full flex justify-center py-2 px-4 border border-transparent text-sm font-medium rounded-md text-white bg-indigo-600 hover:bg-indigo-700 focus:outline-none focus:ring-2 focus:ring-offset-2 focus:ring-indigo-500">
            <span class="absolute left-0 inset-y-0 flex items-center pl-3">
              <!-- <LockClosedIcon class="h-5 w-5 text-indigo-500 group-hover:text-indigo-400" aria-hidden="true" /> -->
            </span>
            登录
          </button>
        </div>
      </form>
    </div>
  </div>
</template>

<script lang="ts" setup>
import { ref,onMounted } from 'vue';
import { wx } from '@/stores/axios';
import {RouterLink} from 'vue-router';
import CryptoJS from 'crypto-js';
import { useRouter } from 'vue-router';

const username = ref('')
const password = ref('')
const router = useRouter()
const handleSubmit = () => {
  wx("register",{username: username.value, password: CryptoJS.MD5(password.value).toString()}).then(res => {
    if(res.code == 200) {
      router.push('/wx/login')
    }
  })
}
onMounted(()=>{
  wx("getInfo").then(res => {
    if(res.code == 200) {
      console.log("Already logged in")
      router.push('/')
    }else{
      console.log("Not login")
    }
  })
})
</script>
