#include <iostream>
#include <curl/curl.h>

// CURL 응답을 처리하기 위한 콜백 함수
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((std::string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

std::string getIP() {
    CURL* curl;
    CURLcode res;
    std::string readBuffer;

    // curl 초기화
    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    
    if(curl) {
        // 공인 IP를 반환하는 API로 요청
        curl_easy_setopt(curl, CURLOPT_URL, "https://api.ipify.org");
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);
        
        res = curl_easy_perform(curl); // 요청 보내기

        if(res != CURLE_OK) {
            std::cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << std::endl;
        } else {
            return readBuffer; // 공인 IP 반환
        }

        curl_easy_cleanup(curl);
    }

    curl_global_cleanup();
    return "";
}
