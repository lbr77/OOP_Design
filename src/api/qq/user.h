#ifndef _QQ_USER_H_
#define _QQ_USER_H_
/**
 * User Router
 */
#include "../../http/json.h"
#include "../../utils/log.h"
#include "../../class/user.h"
#include "../../dao/qq/user_dao.h"
class QQUserHandler: public JsonHandler {
public:
    HTTPResponse handle(HTTPRequest &request){
        
    }
};
#endif 