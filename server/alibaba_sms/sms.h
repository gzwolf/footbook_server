//
// Created by YangGuang on 2019-04-09.
//

#ifndef CAMPUS_CHAT_SMS_H
#define CAMPUS_CHAT_SMS_H

#include <string>

#include "server/status.h"
#include "base/macor.h"

namespace footbook {

class SMS {
 public:
    static SMS* GetInstance();

    Status Send(const std::string &phone_number,
              const std::string &code);

 private:
    // 阿里云短信服务的http请求参数结构
    struct SMSHttpArg {
        // http 服务器地址
        std::string address;
        // 端口号
        int port;
        // 请求的签名, 即最终生成的签名结果值.
        std::string signature;
        // 访问密钥， AccessKey用于调用API
        std::string access_key_id;
        std::string access_key_secret;
        // API的名称
        std::string action;
        // 返回参数的语言类型，取值范围: json | xml, 默认json
        std::string format;
        // API支持的RegionID，如短信API的值为：cn-hangzhou
        std::string region_id;
        // 签名方式， 取值范围: HMAC-SHA1
        std::string signature_method;
        // 签名唯一随机数。用于防止网络重放攻击，建议您每一次请求都使用不同的随机数。
        std::string signature_nonce;
        // 签名版本号，默认为1.0
        std::string signature_version;
        // 请求的时间戳。按照ISO8601 标准表示，并需要使用UTC时间，格式为yyyy-MM-ddTHH:mm:ssZ
        // 示例：2018-01-01T12:00:00Z 表示北京时间 2018 年 01 月 01 日 20 点 00 分 00 秒。
        std::string time_stamp;
        // API 的版本号，格式为 YYYY-MM-DD。取值范围：2017-05-25。
        std::string version;
        // 短信接收号码,支持以逗号分隔的形式进行批量调用，
        // 批量上限为1000个手机号码,批量调用相对于单条调用及时性稍有延迟,
        // 验证码类型的短信推荐使用单条调用的方式
        std::string phone_number;
        // 短信签名
        std::string sign_name;
        // 短信模版id
        std::string template_code;
        // 短信模版参数
        std::string template_param;
        // 外部流水扩展字段
        std::string out_id;

        explicit SMSHttpArg(const std::string& number, const std::string& code)
            : phone_number(number) {
            template_param = "{\"code\":\"" + code + "\"}";
        }
    };

    void InitForAliyuSmsHttp(SMSHttpArg *sms_http);

    void GeneratorURL(const SMSHttpArg &sms_http, std::string *url);

    //SMS() = default;
    //DISALLOW_COPY_AND_ASSIGN(SMS);
};

}   // namespace footbook


#endif //CAMPUS_CHAT_SMS_H
