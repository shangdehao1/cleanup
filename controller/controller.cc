// Copyright (c) 2014 Baidu.com, Inc. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include <memory>
#include "../stream/endpoint.h"
#include "controller.h"
#include "controller_impl.h"

namespace hdcs {
namespace networking {

Controller::Controller()
    : _impl(new ControllerImpl()) // the actual Controller implement.
{
}

Controller::~Controller()
{
}

std::string Controller::LocalAddress() const
{
    //return EndpointToString(_impl->LocalEndpoint());
}

std::string Controller::RemoteAddress() const
{
    //return EndpointToString(_impl->RemoteEndpoint());
}

// re-create a new rpcControllerImp object.
void Controller::Reset()
{
    _impl.reset(new ControllerImpl());
}

void Controller::SetTimeout(int64_t timeout_in_ms)
{
    _impl->SetTimeout(timeout_in_ms);
}

int64_t Controller::Timeout() const
{
    return _impl->Timeout();
}

/*
void Controller::SetRequestCompressType(CompressType compress_type)
{
    _impl->SetRequestCompressType(compress_type);
}

void Controller::SetResponseCompressType(CompressType compress_type)
{
    _impl->SetResponseCompressType(compress_type);
}
*/

bool Controller::Failed() const
{
    return _impl->Failed();
}

int Controller::ErrorCode() const
{
    return _impl->ErrorCode();
}

std::string Controller::ErrorText() const
{
    return _impl->ErrorText();
}

bool Controller::IsRequestSent() const
{
    return _impl->IsRequestSent();
}

int64_t Controller::SentBytes() const
{
    return _impl->SentBytes();
}

void Controller::StartCancel()
{
    _impl->StartCancel();
}

bool Controller::IsHttp() const
{
    return _impl->IsHttp();
}

const std::string& Controller::HttpPath() const
{
    return _impl->HttpPath();
}

const std::map<std::string, std::string>& Controller::HttpQueryParams() const
{
    return _impl->HttpQueryParams();
}

const std::map<std::string, std::string>& Controller::HttpHeaders() const
{
    return _impl->HttpHeaders();
}

void Controller::SetFailed(const std::string& reason)
{
    _impl->SetFailed(reason);
}

bool Controller::IsCanceled() const
{
    return _impl->IsCanceled();
}

//void Controller::NotifyOnCancel(google::protobuf::Closure* callback)
void Controller::NotifyOnCancel()
{
    //_impl->NotifyOnCancel(callback);
    _impl->NotifyOnCancel();
}

}
}
