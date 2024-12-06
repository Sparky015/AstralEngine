/**
* @file ClientManager.h
* @author Andrew Fagan
* @date 12/5/2024
*/

#pragma once


namespace Client {

    class ClientTest
    {
    public:
        virtual void Init();

    };

    class Client : public ClientTest
    {
    public:
        void Init() override;
    };

    void Init();
    void Update();

}