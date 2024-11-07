#include <iostream>
#include <string>
#include <curl/curl.h>
#include <nlohmann/json.hpp>

using namespace std;

// Define a callback function to handle the response data from CURL
static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    ((string*)userp)->append((char*)contents, size * nmemb);
    return size * nmemb;
}

int main() {
    // Initialize libcurl
    CURL* curl;
    CURLcode res;
    string readBuffer;

    curl = curl_easy_init();
    if(curl) {
        // Set the URL of the D&B API endpoint
        string url = "https://developer-api-testmode.dnb.no/test-customers/v0";  // Adjust endpoint based on your needs

        // Replace with your actual API key
        string apiKey = "2e69a2c2455d4244ab44fbfdd2f489c6";

        
        // Set up headers including the API key for authorization
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, ("Authorization: Bearer " + apiKey).c_str());
        headers = curl_slist_append(headers, "Content-Type: application/json");

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);


        // Set callback to capture response
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &readBuffer);

        // Perform the request and check for errors
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
            cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
        } else {
            // Parse the response as JSON
            try {
                auto jsonResponse = nlohmann::json::parse(readBuffer);
                cout << "Response JSON:" << endl;
                cout << jsonResponse.dump(4) << endl;  // Pretty print with indentation
            } catch (nlohmann::json::parse_error& e) {
                cerr << "JSON Parsing Error: " << e.what() << endl;
            }
        }

        // Clean up
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }
    return 0;
}