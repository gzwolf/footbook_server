//
// Created by YangGuang on 2019-04-09.
//

#include "sms.h"

#include <ctime>
#include <sstream>
#include <openssl/hmac.h>

#include "base/singleton.h"
#include "glog/logging.h"
#include "footbook/config.h"
#include "footbook/http/http_boost.h"

namespace {

std::string ReplaceAll(std::string str,std::string old_value,
        std::string new_value) {
    while(true){
        std::string::size_type  pos(0);
        if((pos = str.find(old_value)) != std::string::npos)
        str.replace(pos,old_value.length(),new_value);
        else
        break;
    }

    return str;
}

std::string Sgin(char key[], const char * data) {
    unsigned char digest[EVP_MAX_MD_SIZE + 1] = {'\0'};
    unsigned int digest_len = 0;
    HMAC(EVP_sha1(), key, strlen(key) + 1, (unsigned char*)data,
            strlen(data), digest, &digest_len);

    char mdString[41] = {'\0'};

    for(int i = 0; i < digest_len; i++)
        sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    return footbook::Base64Encode(digest,digest_len);
}

}   // namespace

namespace footbook {

SMS* SMS::GetInstance() {
    return base::Singleton<SMS,
            base::LeakySingletonTraits<SMS>>::get();
}

Status SMS::Send(const std::string &phone_number, const std::string &code) {
    SMSHttpArg sms_http(phone_number, code);
    InitForAliyuSmsHttp(&sms_http);
    std::string url;
    GeneratorURL(sms_http, &url);
    LOG(INFO) << "URL = " << url;
    std::string responce;
    Status status = http::Get(url, &responce);
    if (!status.ok())
        return status;
    // 处理响应数据
    LOG(INFO) << "responce = " <<responce;
    return Status::Ok();
}

void SMS::InitForAliyuSmsHttp(SMS::SMSHttpArg *sms_http) {
    sms_http->access_key_id = "LTAI7cLVy2ex20Oi";
    sms_http->access_key_secret = "8bo3BLsSbOOw4fQJVRaoFSXzf2Ebl8&";
    sms_http->action = "SendSms";
    sms_http->signature_method = "HMAC-SHA1";
    sms_http->signature_version = "1.0";
    sms_http->version = "2017-05-25";
    sms_http->region_id = "cn-hangzhou";
    sms_http->sign_name = "footbook";
    sms_http->template_code = "SMS_98465010";
    //sms_http->template_param = "{\"code\":\"362387\"}";
    sms_http->address = "dysmsapi.aliyuncs.com";
    sms_http->port = 80;

    char time_stamp[30] = {0};
    char signature_nonce[16] = {0};

    time_t raw_time;
    struct tm* time_info;
    time(&raw_time);
    time_info = gmtime(&raw_time);
    strftime(time_stamp,sizeof(time_stamp),"%Y-%m-%dT%H:%M:%SZ",time_info);
    sms_http->time_stamp = time_stamp;
    snprintf(signature_nonce, 16 - 1 ,"%ld",time(nullptr));
    sms_http->signature_nonce = signature_nonce;
}

void SMS::GeneratorURL(const SMS::SMSHttpArg &sms_http, std::string *url) {
    std::ostringstream ss;
    ss << UrlEncode(std::string("AccessKeyId")) << "="  \
       << UrlEncode(sms_http.access_key_id) << "&"      \
       << UrlEncode(std::string("Action")) << "="       \
       << UrlEncode(sms_http.action) << "&"             \
       << UrlEncode(std::string("PhoneNumbers")) << "=" \
       << UrlEncode(sms_http.phone_number) << "&"       \
       << UrlEncode(std::string("RegionId")) << "="     \
       << UrlEncode(sms_http.region_id) << "&"          \
       << UrlEncode(std::string("SignName")) << "="     \
       << UrlEncode(sms_http.sign_name) << "&"    \
       << UrlEncode(std::string("SignatureMethod")) << "="  \
       << UrlEncode(sms_http.signature_method) << "&"       \
       << UrlEncode(std::string("SignatureNonce")) << "="   \
       << UrlEncode(sms_http.signature_nonce) << "&"        \
       << UrlEncode(std::string("SignatureVersion")) << "=" \
       << UrlEncode(sms_http.signature_version) << "&"      \
       << UrlEncode(std::string("TemplateCode")) << "="     \
       << UrlEncode(sms_http.template_code) << "&"          \
       << UrlEncode(std::string("TemplateParam")) << "="    \
       << UrlEncode(sms_http.template_param) << "&"         \
       << UrlEncode(std::string("Timestamp")) << "="        \
       << UrlEncode(sms_http.time_stamp) << "&"             \
       << UrlEncode(std::string("Version")) << "="          \
       << UrlEncode(sms_http.version);

    std::string str_url = "GET&" + UrlEncode("/") + "&" + UrlEncode(ss.str());
    std::string str_url2 = ReplaceAll(ReplaceAll(ReplaceAll(
            str_url, "%7E", "~"), "*", "%2A"), "+", "%20");
    LOG(INFO) << str_url2;
    std::string sgin = Sgin(const_cast<char*>(
            sms_http.access_key_secret.c_str()),str_url2.c_str());
    std::string str_url3 = ReplaceAll(ReplaceAll(
            ReplaceAll(UrlEncode(sgin), "%7E", "~"), "*", "%2A"), "+", "%20");
    LOG(INFO) << str_url3;
    url->clear();
    *url = "http://" + sms_http.address + "/?Signature=" + str_url3 + "&" + ss.str();
}




}