#include <iostream>
#include <string>
#include <curl/curl.h>
#include <unistd.h>  // Pour sleep

// Fonction de rappel pour écrire la réponse HTTP dans une chaîne
size_t WriteCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append(static_cast<char*>(contents), totalSize);
    return totalSize;
}

int main()
{
	CURL* curl;
	CURLcode res;
	
	curl = curl_easy_init();
	if (curl)
	{
		std::string response;
		const char* url = "https://api.openai.com/v1/chat/completions";
		const char* json_data = 
		"{"
		"    \"messages\": ["
		"        {"
		"            \"role\": \"user\","
		"            \"content\": \"salut ca va ?\""
		"        }"
		"    ],"
		"    \"model\": \"gpt-3.5-turbo\""
		"}";
		struct curl_slist* headers = NULL;
		headers = curl_slist_append(headers, "Content-Type: application/json");
		headers = curl_slist_append(headers, "Authorization: Bearer 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");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		bool is_response_ready = true;
		bool is_request_ready = false;
		int i = 0;
		std::string response_data;
		while (1)
		{
			if (is_response_ready == true)
			{
				curl_easy_setopt(curl, CURLOPT_URL, url);
				if (i++== 0)
				{
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);
					std::cout << "jsonData =- " << std::endl;
					std::cout << json_data << std::endl;
				}
				else
				{
					struct curl_slist* headers_with_content_type = NULL;
					headers_with_content_type = curl_slist_append(headers_with_content_type, "Content-Type: application/json");
					headers_with_content_type = curl_slist_append(headers_with_content_type, "Authorization: Bearer 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");
					curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers_with_content_type);
					curl_easy_setopt(curl, CURLOPT_POSTFIELDS, response_data.c_str()); 
				}
				std::cout << "Sending this response : " << std::endl << response_data << std::endl;
				curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
				curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
				res = curl_easy_perform(curl);
				if (res != CURLE_OK)
				{
					std::cerr << "curl_easy_perform() a échoué : " << curl_easy_strerror(res) << std::endl;
				}
				else
				{
					std::cout << "Réponse reçue : " << std::endl;
					std::cout << response << std::endl;
				}
				response.clear();
				is_request_ready = false;
			}
			while (!is_response_ready)
				sleep(1);

			if (is_request_ready == false)
			{
				std::string user_input;
        		std::cout << "Vous : ";
        		std::getline(std::cin, user_input);
				response_data = "{"
				"    \"messages\": ["
				"        {"
				"            \"role\": \"user\","
				"            \"content\": \"" + user_input + "\""
				"        }"
				"    ],"
				"    \"model\": \"gpt-3.5-turbo\""
				"}";
				is_response_ready = true;
		
			}
		}
        curl_easy_cleanup(curl);
    }
	else {
        std::cerr << "Impossible d'initialiser cURL." << std::endl;
        return 1;
    }

    return 0;
}