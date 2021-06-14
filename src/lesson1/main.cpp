#include <httpserver.hpp> // наша великолепная библиотека
#include <iostream>      // std::cout

// добавим в наш мир красок
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define WHITE   "\033[37m"      /* White */

// здесь мы храним адрес, куда перенаправим пользователя
std::string savedUrl;

// долой длинные имена
using http_response = std::shared_ptr<httpserver::http_response>;
using http_request  = httpserver::http_request;

// логика запоминания ссылок
class Save_resource: public httpserver::http_resource {

    const http_response render_POST(const http_request& req) {
        std::cout << GREEN  << std::endl << "Вызов Save_resource::render функции" << std::endl;

        std::string body = req.get_content();
        std::cout << YELLOW << "Тело POST запроса = " << BLUE << body << std::endl;
        savedUrl = body;

        std::string response_text = "Упешно добавлен redirect на " + savedUrl + "\n";
        httpserver::string_response* response = new httpserver::string_response(response_text, 200);

        std::cout << RED << "Вызов функции Save_resource::render завершен" << std::endl;
        return http_response(response);
    }

};

// логика перенаправления пользователя
class Redirect_resource: public httpserver::http_resource {

    const http_response render_GET(const httpserver::http_request& req) {
        std::cout << GREEN  << std::endl << "Вызов Redirect_resource::render функции" << std::endl;

        httpserver::string_response* response = new httpserver::string_response("", 302);
        response->with_header("Location", savedUrl);

        std::cout << RED << "Вызов функции Redirect_resource::render завершен" << std::endl;
        return http_response(response);
    }

};

// страница с ошибкой
class Error_resource: public httpserver::http_resource {
    const http_response render_GET(const httpserver::http_request& req) {
        std::cout << GREEN  << std::endl << "Вызов Error_resource::render функции" << std::endl;

        httpserver::string_response* response = new httpserver::string_response("Requested resource is not found", 404);

        std::cout << RED << "Вызов функции Error_resource::render завершен" << std::endl;
        return http_response(response);
    }

};

int main() {
    // создаем webserver
    httpserver::webserver ws = httpserver::create_webserver(8080);

    // вспомогательная переменная, в которой мы будем хранить результат создания веб сервера
    bool successStatus = false;

    // добавляем обработчик на запоминание URL-а
    Save_resource sr;
    successStatus = ws.register_resource("/save", &sr);
    if (successStatus == false) {
        std::cout << WHITE << "Не удалось создать обработчик адреса 'save'" << std::endl;
        return 1;
    }

    // добавляем обработчик на перенаправление пользователя
    Redirect_resource rdr;
    successStatus = ws.register_resource("/redirect", &rdr);
    if (successStatus == false) {
        std::cout << WHITE << "Не удалось создать обработчик адреса 'redirect'" << std::endl;
        return 1;
    }

    Error_resource err;
    successStatus = ws.register_resource("/error", &err);
    if (successStatus == false) {
        std::cout << WHITE << "Не удалось создать обработчик адреса 'error'" << std::endl;
        return 1;
    }

    ws.start(true);
    return 0;
}