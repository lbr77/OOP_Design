import axios from "axios";

// 创建axios实例
const axiosInstance = axios.create({
  baseURL: "http://localhost:8080",
  validateStatus: () => true,
});

// 配置跨域携带凭证
axiosInstance.defaults.withCredentials = true;

// 定义通用响应类型
interface ApiResponse<T = any> {
  code: number;
  data: T;
  message: string;
}

/**
 * QQ接口通用请求方法
 * @param action 操作类型
 * @param data 请求数据
 */
export const qq = async <T = any>(
  action: string,
  data: Record<string, any> = {}
): Promise<T> => {
  data.action = action;
  console.log(data);
  return await axiosInstance.post("/qq/api", data).then(data=>data.data);
};

export default axiosInstance;
