#ifndef CW_CLI_H
#define CW_CLI_H
#pragma once

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "..\Shared\PCCommunication.h"
#include "..\Shared\WirelessConnection.h"

class CLI
{
public:
    CLI(PCCommunication& _pc, WirelessConnection& _wc):pc(_pc), wc(_wc) {}

    void Update()
    {
        if (pc.Update())
        {
            Interpret(pc.ReadCommand());
        }
    }

    void Interpret(std::string&& cmd)
    {
        std::vector<std::string> tokens{ Tokenize(cmd) };
        if (cmdMap.end() != cmdMap.find(tokens[0]))
        {
            (this->*cmdMap[tokens[0]])(tokens);
        }
        else {
            printf("Bad command: %s\n", cmd.c_str());
        }
    }

private:
    std::vector<std::string> Tokenize(std::string& line)
    {
        std::vector<std::string> tokens;
        std::istringstream iss{ line };
        std::string token;
        while (iss >> token)
        {
            tokens.emplace_back(token);
        }
        return tokens;
    }

    template <class T> T convert(const std::string& token) {
        std::istringstream iss{token};
        T value;
        iss >> value;
        return value;
    }

    bool validate(const std::vector<std::string>& tokens, unsigned count)
    {
        bool valid = (tokens.size() == count);
        if (!valid)
            pc.Write("Invalid command syntax\n");
        return valid;
    }

    void pair(const std::vector<std::string>& tokens) {
        pc.Write("Pairing\n");
        wc.HostPair();
    }

    void ack(const std::vector<std::string>& tokens)
    {
        if (validate(tokens, 2)) {
            unsigned seq = convert<unsigned>(tokens[1]);
            printf("Sending Ack %d\n", seq);
            wc.Ack(seq);
        }
    }

    void bad(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned seq = convert<unsigned>(tokens[1]);
            printf("Sending bad commend %d\n", seq);
            wc.BadCommand(seq);
        }
    }

    void notimpl(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned seq = convert<unsigned>(tokens[1]);
            printf("Sending NotImpl %d\n", seq);
            wc.NotImplemented(WirelessConnection::eProtocolCommand(seq));
        }
    }

    void ok (const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned seq = convert<unsigned>(tokens[1]);
            printf("Sending OK: %d\n", seq);
            wc.OK(seq);
        }
    }

    void drop(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned seq = convert<unsigned>(tokens[1]);
            printf("Sending DROP: %d\n", seq);
            wc.Drop(seq);
        }
    }

    void echo(const std::vector<std::string>& tokens) {
        std::string line { tokens[1] };
        for (auto iter = ++(++tokens.begin()); iter != tokens.end(); ++iter)
        {
            line += ' ' + *iter;
        }
        printf("Ecohing: \"%s\"\n", line.c_str());
        wc.Echo(line.c_str(), line.length());
    }

    void ping(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned seq = convert<unsigned>(tokens[1]);
            printf("Sending PING: %d\n", seq);
            wc.Ping(seq);
        }
    }

    void waiting(const std::vector<std::string>& tokens) {
        printf("Sending WAIT\n");
        wc.Waiting();
    }

    void leftbias(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            float param = convert<float>(tokens[1]);
            printf("Sending Left Bias: %f\n", param);
            wc.SetLeftWheelBias(param);
        }
    }

    void rightbias(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            float param = convert<float>(tokens[1]);
            printf("Sending Right Bias: %f\n", param);
            wc.SetRightWheelBias(param);
        }
    }

    void forward(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending forward: %d ms\n", param);
            wc.Forward(param);
        }
    }

    void reverse(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending forward: %d ms\n", param);
            wc.Reverse(param);
        }
    }

    void turnld(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending Turn Left Degrees: %d\n", param);
            wc.TurnLeftDegrees(param);
        }
    }
    void turnrd(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending Turn Right Degrees: %d\n", param);
            wc.TurnRightDegrees(param);
        }
    }
    void turnlt(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending Turn Left Timed: %d\n", param);
            wc.TurnLeftTimed(param);
        }
    }
    void turnrt(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2))
        {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending Turn Right Timed: %d\n", param);
            wc.TurnRightTimed(param);
        }
    }

    void speed(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            float param = convert<float>(tokens[1]);
            printf("Sending speed: %f\n", param);
            wc.SetSpeed(param);
        }
    }

    void getspeed(const std::vector<std::string>& tokens) {
        pc.Write("Sending GetSpeed\n");
        wc.GetSpeed();
    }

    void go(const std::vector<std::string>&){
        pc.Write("Sending GO\n");
        wc.Go();
    }

    void stop(const std::vector<std::string>& tokens) {
        pc.Write("Sending STOP\n");
        wc.Stop();
    }

    void setpos(const std::vector<std::string>& tokens) {
        if (validate(tokens, 3)) {
            unsigned xPos = convert<unsigned>(tokens[1]);
            unsigned yPos = convert<unsigned>(tokens[2]);
            printf("Sending SetPos: %d, %d\n", xPos, yPos);
            wc.SetPosition(xPos, yPos);
        }
    }
    
    void getpos(const std::vector<std::string>& tokens) {
        pc.Write("Sending GetPos\n");
        wc.GetPosition();
    }

    void seto(const std::vector<std::string>& tokens) {
        if (validate(tokens, 2)) {
            float param = convert<float>(tokens[1]);
            printf("Sending Left Bias: %f\n", param);
            wc.SetOrientation(param);
        }
    }

    void geto(const std::vector<std::string>& tokens) {
        pc.Write("Sending GetOrientation\n");
        wc.GetOrientation();
    }

    void setdst(const std::vector<std::string>& tokens) {
        if (validate(tokens, 3)) {
            unsigned xPos = convert<unsigned>(tokens[1]);
            unsigned yPos = convert<unsigned>(tokens[2]);
            printf("Sending Set Dest: %d, %d\n", xPos, yPos);
            wc.SetDestination(xPos, yPos);
        }
    }

    void getdst(const std::vector<std::string>& tokens) {
        pc.Write("Sending GetDest\n");
        wc.GetDestination();
    }
    void reqpath(const std::vector<std::string>& tokens) {
        pc.Write("Sending Request Path\n");
        wc.RequestPath();
    }

    void pathnode(const std::vector<std::string>& tokens) {
        if (validate(tokens, 4))
        {
            unsigned step{0}, nodeX{0}, nodeY{0};
            step = convert<unsigned>(tokens[1]);
            nodeX = convert<unsigned>(tokens[2]);
            nodeY = convert<unsigned>(tokens[3]);
            printf("Sending path node: Step: %d, (%d, %d)\n", step, nodeX, nodeY);
            wc.RelayPath(step, nodeX, nodeY);
        }
    }

    void obstacle(const std::vector<std::string>& tokens) {
        if (validate(tokens, 3)) {
            unsigned xPos = convert<unsigned>(tokens[1]);
            unsigned yPos = convert<unsigned>(tokens[2]);
            printf("Sending Obstacle: %d, %d\n", xPos, yPos);
            wc.NotifyObstacle(xPos, yPos);
        }
    }

    void turntime(const std::vector<std::string>& tokens)
    {
        if (validate(tokens, 2)) {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending forward: %d ms\n", param);
            wc.TurnTime(param);
        }
    }

    void forwardtime(const std::vector<std::string>& tokens)
    {
        if (validate(tokens, 2)) {
            unsigned param = convert<unsigned>(tokens[1]);
            printf("Sending forward: %d ms\n", param);
            wc.ForwardTime(param);
        }
    }

    void step(const std::vector<std::string>& tokens)
    {
        if (validate(tokens, 1)) {
            printf("Sending STEP\n");
            wc.Step();
        }
    }

    std::map<std::string, void(CLI::*)(const std::vector<std::string>&)> cmdMap {
        { "pair", &CLI::pair },
        { "ack", &CLI::ack },
        { "bad", &CLI::bad },
        { "notimpl", &CLI::notimpl },
        { "ok", &CLI::ok },
        { "drop", &CLI::drop },
        { "echo", &CLI::echo },
        { "ping", &CLI::ping },
        { "waiting", &CLI::waiting },
        { "leftbias", &CLI::leftbias },
        { "rightbias", &CLI::rightbias },
        { "forward", &CLI::forward },
        { "reverse", &CLI::reverse },
        { "turnld", &CLI::turnld },
        { "turnrd", &CLI::turnrd },
        { "turnlt", &CLI::turnlt },
        { "turnrt", &CLI::turnrt },
        { "speed", &CLI::speed },
        { "getspeed", &CLI::getspeed },
        { "go", &CLI::go },
        { "stop", &CLI::stop },
        { "setpos", &CLI::setpos },
        { "getpos", &CLI::getpos },
        { "seto", &CLI::seto },
        { "geto", &CLI::geto },
        { "setdst", &CLI::setdst },
        { "getdst", &CLI::getdst },
        { "reqpath", &CLI::reqpath },
        { "pathnode", &CLI::pathnode },
        { "obstacle", &CLI::obstacle },
        { "turntime", &CLI::turntime },
        { "forwardtime", &CLI::forwardtime },
        { "step", &CLI::step }
    };

    PCCommunication& pc;
    WirelessConnection& wc;
};

#endif // CW_CLI_H
