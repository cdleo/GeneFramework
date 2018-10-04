/*
 * File:   Parameters.cpp
 * Author: cleo
 *
 * Created on July 29, 2014, 2:56 PM
 */

#include <Parameters.h>
#include <BaseUtilities.h>
#include <Log.h>
#include <tinyxml2.h>

#include <iostream>
#include <stdlib.h>

using namespace tinyxml2;

Parameters::Parameters() {
    _bIsInitialized = false;
}

Parameters::~Parameters() throw() {
    
}

bool Parameters::initialize(std::string routeToConfigFile) {
        
    XMLDocument config;
    XMLError error = config.LoadFile( routeToConfigFile.c_str() );
    if (error != XML_SUCCESS) {
        LOG->LogMessage(LOG_WARNING, "[ERROR] Unable to load configuration file (%s). Error code: %d", routeToConfigFile.c_str(), error);
        return false;
    }
    
    XMLElement* root = config.RootElement();
    XMLNode* node = root->FirstChild();
    while (node != NULL) {
        if (node->ToComment() == NULL) {  //No se procesan los comentarios
            XMLElement* element = node->ToElement();
            if (node->NoChildren()) {
                this->setValue( element->Value(), element->Attribute("value") );
            } else {
                XMLNode* childNode = node->FirstChild();
                while (childNode != NULL) {  
                    if (childNode->ToComment() == NULL) { //No se procesan los comentarios
                        XMLElement* childElement = childNode->ToElement();
                        this->addValue( element->Value(), childElement->Attribute("value") );
                    }
                    childNode = childNode->NextSibling();
                }
            }
        }
        node = node->NextSibling();
    }
    
    _bIsInitialized = true;
    return true;
}

bool Parameters::getValue(std::string key, long& value) {
    
    std::vector<std::string> values;
    if (!getValue(key, values))
        return false;
    
    if (values.empty())
        return false;
    
    if (!UTILITIES->isNumeric(values.at(0)))
        return false;
    
    value = atol( values.at(0).c_str() );
    return true;
}

bool Parameters::getValue(std::string key, std::string& value){
    
    std::vector<std::string> values;
    if (!getValue(key, values))
        return false;
    
    if (values.empty())
        return false;
    
    value.assign( values.at(0) );
    return true;
}

bool Parameters::getValue(std::string key, std::vector<std::string>& values){
    
    if (parameters_.find(key) == parameters_.end())
        return false;
    
    values.clear();
    std::vector<std::string> aux = parameters_.find(key)->second;
    std::copy(aux.begin(), aux.end(), std::back_inserter(values));
    return true;
}

bool Parameters::addValue(std::string key, std::string value){
    
    std::vector<std::string> values;
    if (parameters_.find(key) == parameters_.end())
        return setValue(key, value);
        
    values = parameters_.at(key);
    values.push_back( value );
    
    return this->setValue(key, values);
}

bool Parameters::setValue(std::string key, std::string value){
    
    std::vector<std::string> values;
    values.push_back( value );

    return this->setValue(key, values);
}

bool Parameters::setValue(std::string key, std::vector<std::string> values){

    parameters_[key] = values;
    return true;
}

bool Parameters::existValue(std::string key, std::string value){

    std::vector<std::string> values;
    if (parameters_.find(key) == parameters_.end())
        return false;    
    
    values = parameters_.at(key);
    for (std::vector<std::string>::iterator it = values.begin(); it != values.end(); it++){
        if (it->compare(value) == 0) return true;
    }    
    
    return false;
}


