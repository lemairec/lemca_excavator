#include "my_qt_network.hpp"
#include "include_qt.hpp"
#include "../../framework.hpp"
#include <sstream>
#include <QMetaEnum>
#include <QSerialPortInfo>
#include <QFile>
#include <QFileInfo>
#include <QHttpMultiPart>

#include "../gps_widget.hpp"

#include "environnement.hpp"
#include "../../util/directory_manager.hpp"

QNetworkAccessManager * manager;
MyQTNetwork::MyQTNetwork(){
    manager = new QNetworkAccessManager();
    
    QObject::connect(manager, &QNetworkAccessManager::finished, this,&MyQTNetwork::handleNetwork);
}
void MyQTNetwork::initOrLoad(Config & config){
    m_host_url = "https://maplaine.fr";
    //m_host_url = "http://localhost:4000";
    m_company = QString::fromStdString(config.m_company);

    // Auto-login : si des identifiants Land Manager sont enregistres, on verifie
    if(!config.m_landmanager_token.empty() ||
       (!config.m_landmanager_user.empty() && !config.m_landmanager_password.empty())){
        checkLogin(config);
    }
};

void MyQTNetwork::closeAll(){
    INFO("###close all");
}

std::string m_name;
void MyQTNetwork::handleNetwork(QNetworkReply *reply) {
    if (reply->error()) {
        std::string error = reply->errorString().toUtf8().constData();
        WARN(error);
        //Framework::instance().addError(error);
        Framework::instance().m_synchro.erreurSynchro(error, reply->readAll().toStdString());
        return;
    } else {
        std::string s = reply->readAll().toStdString();
        INFO(m_name);
        if(m_name == "parcelles"){
            Framework::instance().m_synchro.onParcellesResponse(s);
        } else if(m_name == "balises"){
            Framework::instance().m_synchro.onBalisesResponse(s);
        } else {
            Framework::instance().m_synchro.onParcelleResponse(m_name, s);
        }
    }
};



void MyQTNetwork::handleErrorGps(QSerialPort::SerialPortError error){
    
}

void MyQTNetwork::getParcelles(){
    m_name = "parcelles";
    QNetworkRequest request;

    QString url = m_host_url + "/api/autosteer/parcelles?company=" + m_company;
    INFO(url.toUtf8().constData());
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setHeader(QNetworkRequest::ServerHeader, "application/json");

    manager->get(networkRequest);
    
}

void MyQTNetwork::getParcelle(const std::string & name){
    m_name = name;
    
    
    QString url = m_host_url + "/api/autosteer/parcelle/" + QString::fromStdString(name) + "?company=" + m_company;
    
    INFO(url.toUtf8().constData());
    QNetworkRequest request;
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setHeader(QNetworkRequest::ServerHeader, "application/json");

    manager->get(networkRequest);
}

void MyQTNetwork::saveParcelle(const std::string & name, const std::string & json){
    m_name = name;
    QNetworkRequest request;

    QString url = m_host_url + "/api/autosteer/parcelle?company=" + m_company;
    QUrl serviceUrl = QUrl(url);
    QByteArray postData;

    QUrlQuery query;
    
    INFO("save " << name);
    query.addQueryItem("parcelle", QString::fromStdString(json));
    
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    
    manager->post(networkRequest,postData);
}

static QNetworkAccessManager * soil_manager = nullptr;

bool MyQTNetwork::uploadSoil(Config & config, const std::string & file_path){
    QString token = QString::fromStdString(config.m_landmanager_token);
    QString user  = QString::fromStdString(config.m_landmanager_user);
    QString pass  = QString::fromStdString(config.m_landmanager_password);
    if(token.isEmpty() && (user.isEmpty() || pass.isEmpty())){
        WARN("Land Manager: renseignez 'landmanager_user'/'landmanager_password' (ou 'landmanager_token') dans le .ini");
        return false;
    }

    QFile * file = new QFile(QString::fromStdString(file_path));
    if(!file->open(QIODevice::ReadOnly)){
        WARN("soil.txt introuvable : " << file_path);
        delete file;
        return false;
    }

    if(!soil_manager){
        soil_manager = new QNetworkAccessManager();
        QObject::connect(soil_manager, &QNetworkAccessManager::finished, this, &MyQTNetwork::handleSoilUpload);
    }

    QHttpMultiPart * multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QHttpPart filePart;
    filePart.setHeader(QNetworkRequest::ContentTypeHeader, QVariant("text/plain"));
    QFileInfo fi(QString::fromStdString(file_path));
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
        QVariant("form-data; name=\"file\"; filename=\"" + fi.fileName() + "\""));
    filePart.setBodyDevice(file);
    file->setParent(multiPart); // libere le QFile avec le multipart
    multiPart->append(filePart);

    auto addField = [&](const QString & name, const std::string & value){
        if(value.empty()) return;
        QHttpPart p;
        p.setHeader(QNetworkRequest::ContentDispositionHeader,
            QVariant("form-data; name=\"" + name + "\""));
        p.setBody(QString::fromStdString(value).toUtf8());
        multiPart->append(p);
    };
    addField("client_name", config.m_landmanager_client);
    addField("field_name", config.m_landmanager_field);

    QString url = QString::fromStdString(config.m_landmanager_url) + "/api/soil/device-upload";
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    if(!token.isEmpty()){
        request.setRawHeader("X-Device-Token", token.toUtf8());
    } else {
        QByteArray creds = (user + ":" + pass).toUtf8().toBase64();
        request.setRawHeader("Authorization", "Basic " + creds);
    }

    QNetworkReply * reply = soil_manager->post(request, multiPart);
    multiPart->setParent(reply); // libere le multipart avec la reponse
    INFO("upload soil -> " << url.toUtf8().constData());
    return true;
}

void MyQTNetwork::handleSoilUpload(QNetworkReply *reply){
    if(reply->error()){
        std::string error = reply->errorString().toUtf8().constData();
        std::string body = reply->readAll().toStdString();
        WARN("upload soil erreur: " << error << " " << body);
    } else {
        std::string s = reply->readAll().toStdString();
        INFO("upload soil ok: " << s);
    }
    reply->deleteLater();
}

static QNetworkAccessManager * lm_check_manager = nullptr;

void MyQTNetwork::checkLogin(Config & config){
    QString token = QString::fromStdString(config.m_landmanager_token);
    QString user  = QString::fromStdString(config.m_landmanager_user);
    QString pass  = QString::fromStdString(config.m_landmanager_password);
    if(token.isEmpty() && (user.isEmpty() || pass.isEmpty())){
        m_lm_connected = false;
        m_lm_checking = false;
        m_lm_status = "identifiants manquants";
        return;
    }

    if(!lm_check_manager){
        lm_check_manager = new QNetworkAccessManager();
        QObject::connect(lm_check_manager, &QNetworkAccessManager::finished, this, &MyQTNetwork::handleLoginCheck);
    }

    m_lm_checking = true;
    m_lm_status = "connexion...";

    QString url = QString::fromStdString(config.m_landmanager_url) + "/api/soil/device-check";
    QUrl serviceUrl = QUrl(url);
    QNetworkRequest request(serviceUrl);
    if(!token.isEmpty()){
        request.setRawHeader("X-Device-Token", token.toUtf8());
    } else {
        QByteArray creds = (user + ":" + pass).toUtf8().toBase64();
        request.setRawHeader("Authorization", "Basic " + creds);
    }
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    lm_check_manager->post(request, QByteArray());
    INFO("check login -> " << url.toUtf8().constData());
}

void MyQTNetwork::handleLoginCheck(QNetworkReply *reply){
    m_lm_checking = false;
    if(reply->error()){
        m_lm_connected = false;
        int code = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if(code == 401){
            m_lm_status = "login/mot de passe invalide";
        } else {
            m_lm_status = "erreur reseau";
        }
        WARN("check login erreur: " << reply->errorString().toUtf8().constData());
    } else {
        m_lm_connected = true;
        std::string s = reply->readAll().toStdString();
        m_lm_status = "connecte";
        INFO("check login ok: " << s);
    }
    reply->deleteLater();
}

void MyQTNetwork::postBalises(const std::string & json){
    m_name = "balises";
    QNetworkRequest request;

    QString url = m_host_url + "/api/autosteer/balises?company=" + m_company;
    QUrl serviceUrl = QUrl(url);
    QByteArray postData;

    QUrlQuery query;
    
    query.addQueryItem("balises", QString::fromStdString(json));
    postData = query.toString(QUrl::FullyEncoded).toUtf8();

    QNetworkRequest networkRequest(serviceUrl);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    
    manager->post(networkRequest,postData);
}

