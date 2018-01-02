// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: meta.proto

#include "meta.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/stubs/port.h>
#include <google/protobuf/stubs/once.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/wire_format_lite_inl.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
namespace hdcs {
namespace networking {
class MetaDefaultTypeInternal {
 public:
  ::google::protobuf::internal::ExplicitlyConstructed<Meta>
      _instance;
} _Meta_default_instance_;
}  // namespace networking
}  // namespace hdcs
namespace protobuf_meta_2eproto {
void InitDefaultsMetaImpl() {
  GOOGLE_PROTOBUF_VERIFY_VERSION;

  ::google::protobuf::internal::InitProtobufDefaults();
  {
    void* ptr = &::hdcs::networking::_Meta_default_instance_;
    new (ptr) ::hdcs::networking::Meta();
    ::google::protobuf::internal::OnShutdownDestroyMessage(ptr);
  }
  ::hdcs::networking::Meta::InitAsDefaultInstance();
}

void InitDefaultsMeta() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &InitDefaultsMetaImpl);
}

::google::protobuf::Metadata file_level_metadata[1];
const ::google::protobuf::EnumDescriptor* file_level_enum_descriptors[1];

const ::google::protobuf::uint32 TableStruct::offsets[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, _has_bits_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, type_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, sequence_id_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, method_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, server_timeout_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, failed_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, error_code_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, reason_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, compress_type_),
  GOOGLE_PROTOBUF_GENERATED_MESSAGE_FIELD_OFFSET(::hdcs::networking::Meta, expected_response_compress_type_),
  4,
  2,
  0,
  6,
  5,
  7,
  1,
  8,
  3,
};
static const ::google::protobuf::internal::MigrationSchema schemas[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
  { 0, 14, sizeof(::hdcs::networking::Meta)},
};

static ::google::protobuf::Message const * const file_default_instances[] = {
  reinterpret_cast<const ::google::protobuf::Message*>(&::hdcs::networking::_Meta_default_instance_),
};

void protobuf_AssignDescriptors() {
  AddDescriptors();
  ::google::protobuf::MessageFactory* factory = NULL;
  AssignDescriptors(
      "meta.proto", schemas, file_default_instances, TableStruct::offsets, factory,
      file_level_metadata, file_level_enum_descriptors, NULL);
}

void protobuf_AssignDescriptorsOnce() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &protobuf_AssignDescriptors);
}

void protobuf_RegisterTypes(const ::std::string&) GOOGLE_PROTOBUF_ATTRIBUTE_COLD;
void protobuf_RegisterTypes(const ::std::string&) {
  protobuf_AssignDescriptorsOnce();
  ::google::protobuf::internal::RegisterAllTypes(file_level_metadata, 1);
}

void AddDescriptorsImpl() {
  InitDefaults();
  static const char descriptor[] GOOGLE_PROTOBUF_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
      "\n\nmeta.proto\022\017hdcs.networking\032\014option.pr"
      "oto\"\307\002\n\004Meta\022(\n\004type\030\001 \002(\0162\032.hdcs.networ"
      "king.Meta.Type\022\023\n\013sequence_id\030\002 \002(\004\022\016\n\006m"
      "ethod\030d \001(\t\022\026\n\016server_timeout\030e \001(\003\022\017\n\006f"
      "ailed\030\310\001 \001(\010\022\023\n\nerror_code\030\311\001 \001(\005\022\017\n\006rea"
      "son\030\312\001 \001(\t\0225\n\rcompress_type\030\254\002 \001(\0162\035.hdc"
      "s.networking.CompressType\022G\n\037expected_re"
      "sponse_compress_type\030\255\002 \001(\0162\035.hdcs.netwo"
      "rking.CompressType\"!\n\004Type\022\013\n\007REQUEST\020\000\022"
      "\014\n\010RESPONSE\020\001"
  };
  ::google::protobuf::DescriptorPool::InternalAddGeneratedFile(
      descriptor, 373);
  ::google::protobuf::MessageFactory::InternalRegisterGeneratedFile(
    "meta.proto", &protobuf_RegisterTypes);
  ::protobuf_option_2eproto::AddDescriptors();
}

void AddDescriptors() {
  static GOOGLE_PROTOBUF_DECLARE_ONCE(once);
  ::google::protobuf::GoogleOnceInit(&once, &AddDescriptorsImpl);
}
// Force AddDescriptors() to be called at dynamic initialization time.
struct StaticDescriptorInitializer {
  StaticDescriptorInitializer() {
    AddDescriptors();
  }
} static_descriptor_initializer;
}  // namespace protobuf_meta_2eproto
namespace hdcs {
namespace networking {
const ::google::protobuf::EnumDescriptor* Meta_Type_descriptor() {
  protobuf_meta_2eproto::protobuf_AssignDescriptorsOnce();
  return protobuf_meta_2eproto::file_level_enum_descriptors[0];
}
bool Meta_Type_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}

#if !defined(_MSC_VER) || _MSC_VER >= 1900
const Meta_Type Meta::REQUEST;
const Meta_Type Meta::RESPONSE;
const Meta_Type Meta::Type_MIN;
const Meta_Type Meta::Type_MAX;
const int Meta::Type_ARRAYSIZE;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

// ===================================================================

void Meta::InitAsDefaultInstance() {
}
#if !defined(_MSC_VER) || _MSC_VER >= 1900
const int Meta::kTypeFieldNumber;
const int Meta::kSequenceIdFieldNumber;
const int Meta::kMethodFieldNumber;
const int Meta::kServerTimeoutFieldNumber;
const int Meta::kFailedFieldNumber;
const int Meta::kErrorCodeFieldNumber;
const int Meta::kReasonFieldNumber;
const int Meta::kCompressTypeFieldNumber;
const int Meta::kExpectedResponseCompressTypeFieldNumber;
#endif  // !defined(_MSC_VER) || _MSC_VER >= 1900

Meta::Meta()
  : ::google::protobuf::Message(), _internal_metadata_(NULL) {
  if (GOOGLE_PREDICT_TRUE(this != internal_default_instance())) {
    ::protobuf_meta_2eproto::InitDefaultsMeta();
  }
  SharedCtor();
  // @@protoc_insertion_point(constructor:hdcs.networking.Meta)
}
Meta::Meta(const Meta& from)
  : ::google::protobuf::Message(),
      _internal_metadata_(NULL),
      _has_bits_(from._has_bits_),
      _cached_size_(0) {
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  method_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_method()) {
    method_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.method_);
  }
  reason_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  if (from.has_reason()) {
    reason_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.reason_);
  }
  ::memcpy(&sequence_id_, &from.sequence_id_,
    static_cast<size_t>(reinterpret_cast<char*>(&compress_type_) -
    reinterpret_cast<char*>(&sequence_id_)) + sizeof(compress_type_));
  // @@protoc_insertion_point(copy_constructor:hdcs.networking.Meta)
}

void Meta::SharedCtor() {
  _cached_size_ = 0;
  method_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  reason_.UnsafeSetDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  ::memset(&sequence_id_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&compress_type_) -
      reinterpret_cast<char*>(&sequence_id_)) + sizeof(compress_type_));
}

Meta::~Meta() {
  // @@protoc_insertion_point(destructor:hdcs.networking.Meta)
  SharedDtor();
}

void Meta::SharedDtor() {
  method_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
  reason_.DestroyNoArena(&::google::protobuf::internal::GetEmptyStringAlreadyInited());
}

void Meta::SetCachedSize(int size) const {
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
}
const ::google::protobuf::Descriptor* Meta::descriptor() {
  ::protobuf_meta_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_meta_2eproto::file_level_metadata[kIndexInFileMessages].descriptor;
}

const Meta& Meta::default_instance() {
  ::protobuf_meta_2eproto::InitDefaultsMeta();
  return *internal_default_instance();
}

Meta* Meta::New(::google::protobuf::Arena* arena) const {
  Meta* n = new Meta;
  if (arena != NULL) {
    arena->Own(n);
  }
  return n;
}

void Meta::Clear() {
// @@protoc_insertion_point(message_clear_start:hdcs.networking.Meta)
  ::google::protobuf::uint32 cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  if (cached_has_bits & 3u) {
    if (cached_has_bits & 0x00000001u) {
      GOOGLE_DCHECK(!method_.IsDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited()));
      (*method_.UnsafeRawStringPointer())->clear();
    }
    if (cached_has_bits & 0x00000002u) {
      GOOGLE_DCHECK(!reason_.IsDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited()));
      (*reason_.UnsafeRawStringPointer())->clear();
    }
  }
  if (cached_has_bits & 252u) {
    ::memset(&sequence_id_, 0, static_cast<size_t>(
        reinterpret_cast<char*>(&error_code_) -
        reinterpret_cast<char*>(&sequence_id_)) + sizeof(error_code_));
  }
  compress_type_ = 0;
  _has_bits_.Clear();
  _internal_metadata_.Clear();
}

bool Meta::MergePartialFromCodedStream(
    ::google::protobuf::io::CodedInputStream* input) {
#define DO_(EXPRESSION) if (!GOOGLE_PREDICT_TRUE(EXPRESSION)) goto failure
  ::google::protobuf::uint32 tag;
  // @@protoc_insertion_point(parse_start:hdcs.networking.Meta)
  for (;;) {
    ::std::pair< ::google::protobuf::uint32, bool> p = input->ReadTagWithCutoffNoLastTag(16383u);
    tag = p.first;
    if (!p.second) goto handle_unusual;
    switch (::google::protobuf::internal::WireFormatLite::GetTagFieldNumber(tag)) {
      // required .hdcs.networking.Meta.Type type = 1;
      case 1: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(8u /* 8 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::hdcs::networking::Meta_Type_IsValid(value)) {
            set_type(static_cast< ::hdcs::networking::Meta_Type >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                1, static_cast< ::google::protobuf::uint64>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      // required uint64 sequence_id = 2;
      case 2: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(16u /* 16 & 0xFF */)) {
          set_has_sequence_id();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::uint64, ::google::protobuf::internal::WireFormatLite::TYPE_UINT64>(
                 input, &sequence_id_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional string method = 100;
      case 100: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(34u /* 802 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_method()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->method().data(), static_cast<int>(this->method().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "hdcs.networking.Meta.method");
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int64 server_timeout = 101;
      case 101: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(40u /* 808 & 0xFF */)) {
          set_has_server_timeout();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int64, ::google::protobuf::internal::WireFormatLite::TYPE_INT64>(
                 input, &server_timeout_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional bool failed = 200;
      case 200: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(64u /* 1600 & 0xFF */)) {
          set_has_failed();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   bool, ::google::protobuf::internal::WireFormatLite::TYPE_BOOL>(
                 input, &failed_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional int32 error_code = 201;
      case 201: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(72u /* 1608 & 0xFF */)) {
          set_has_error_code();
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   ::google::protobuf::int32, ::google::protobuf::internal::WireFormatLite::TYPE_INT32>(
                 input, &error_code_)));
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional string reason = 202;
      case 202: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(82u /* 1618 & 0xFF */)) {
          DO_(::google::protobuf::internal::WireFormatLite::ReadString(
                input, this->mutable_reason()));
          ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
            this->reason().data(), static_cast<int>(this->reason().length()),
            ::google::protobuf::internal::WireFormat::PARSE,
            "hdcs.networking.Meta.reason");
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .hdcs.networking.CompressType compress_type = 300;
      case 300: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(96u /* 2400 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::hdcs::networking::CompressType_IsValid(value)) {
            set_compress_type(static_cast< ::hdcs::networking::CompressType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                300, static_cast< ::google::protobuf::uint64>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      // optional .hdcs.networking.CompressType expected_response_compress_type = 301;
      case 301: {
        if (static_cast< ::google::protobuf::uint8>(tag) ==
            static_cast< ::google::protobuf::uint8>(104u /* 2408 & 0xFF */)) {
          int value;
          DO_((::google::protobuf::internal::WireFormatLite::ReadPrimitive<
                   int, ::google::protobuf::internal::WireFormatLite::TYPE_ENUM>(
                 input, &value)));
          if (::hdcs::networking::CompressType_IsValid(value)) {
            set_expected_response_compress_type(static_cast< ::hdcs::networking::CompressType >(value));
          } else {
            mutable_unknown_fields()->AddVarint(
                301, static_cast< ::google::protobuf::uint64>(value));
          }
        } else {
          goto handle_unusual;
        }
        break;
      }

      default: {
      handle_unusual:
        if (tag == 0) {
          goto success;
        }
        DO_(::google::protobuf::internal::WireFormat::SkipField(
              input, tag, _internal_metadata_.mutable_unknown_fields()));
        break;
      }
    }
  }
success:
  // @@protoc_insertion_point(parse_success:hdcs.networking.Meta)
  return true;
failure:
  // @@protoc_insertion_point(parse_failure:hdcs.networking.Meta)
  return false;
#undef DO_
}

void Meta::SerializeWithCachedSizes(
    ::google::protobuf::io::CodedOutputStream* output) const {
  // @@protoc_insertion_point(serialize_start:hdcs.networking.Meta)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required .hdcs.networking.Meta.Type type = 1;
  if (cached_has_bits & 0x00000010u) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      1, this->type(), output);
  }

  // required uint64 sequence_id = 2;
  if (cached_has_bits & 0x00000004u) {
    ::google::protobuf::internal::WireFormatLite::WriteUInt64(2, this->sequence_id(), output);
  }

  // optional string method = 100;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->method().data(), static_cast<int>(this->method().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "hdcs.networking.Meta.method");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      100, this->method(), output);
  }

  // optional int64 server_timeout = 101;
  if (cached_has_bits & 0x00000040u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt64(101, this->server_timeout(), output);
  }

  // optional bool failed = 200;
  if (cached_has_bits & 0x00000020u) {
    ::google::protobuf::internal::WireFormatLite::WriteBool(200, this->failed(), output);
  }

  // optional int32 error_code = 201;
  if (cached_has_bits & 0x00000080u) {
    ::google::protobuf::internal::WireFormatLite::WriteInt32(201, this->error_code(), output);
  }

  // optional string reason = 202;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->reason().data(), static_cast<int>(this->reason().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "hdcs.networking.Meta.reason");
    ::google::protobuf::internal::WireFormatLite::WriteStringMaybeAliased(
      202, this->reason(), output);
  }

  // optional .hdcs.networking.CompressType compress_type = 300;
  if (cached_has_bits & 0x00000100u) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      300, this->compress_type(), output);
  }

  // optional .hdcs.networking.CompressType expected_response_compress_type = 301;
  if (cached_has_bits & 0x00000008u) {
    ::google::protobuf::internal::WireFormatLite::WriteEnum(
      301, this->expected_response_compress_type(), output);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    ::google::protobuf::internal::WireFormat::SerializeUnknownFields(
        _internal_metadata_.unknown_fields(), output);
  }
  // @@protoc_insertion_point(serialize_end:hdcs.networking.Meta)
}

::google::protobuf::uint8* Meta::InternalSerializeWithCachedSizesToArray(
    bool deterministic, ::google::protobuf::uint8* target) const {
  (void)deterministic; // Unused
  // @@protoc_insertion_point(serialize_to_array_start:hdcs.networking.Meta)
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = _has_bits_[0];
  // required .hdcs.networking.Meta.Type type = 1;
  if (cached_has_bits & 0x00000010u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      1, this->type(), target);
  }

  // required uint64 sequence_id = 2;
  if (cached_has_bits & 0x00000004u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteUInt64ToArray(2, this->sequence_id(), target);
  }

  // optional string method = 100;
  if (cached_has_bits & 0x00000001u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->method().data(), static_cast<int>(this->method().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "hdcs.networking.Meta.method");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        100, this->method(), target);
  }

  // optional int64 server_timeout = 101;
  if (cached_has_bits & 0x00000040u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt64ToArray(101, this->server_timeout(), target);
  }

  // optional bool failed = 200;
  if (cached_has_bits & 0x00000020u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteBoolToArray(200, this->failed(), target);
  }

  // optional int32 error_code = 201;
  if (cached_has_bits & 0x00000080u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteInt32ToArray(201, this->error_code(), target);
  }

  // optional string reason = 202;
  if (cached_has_bits & 0x00000002u) {
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(
      this->reason().data(), static_cast<int>(this->reason().length()),
      ::google::protobuf::internal::WireFormat::SERIALIZE,
      "hdcs.networking.Meta.reason");
    target =
      ::google::protobuf::internal::WireFormatLite::WriteStringToArray(
        202, this->reason(), target);
  }

  // optional .hdcs.networking.CompressType compress_type = 300;
  if (cached_has_bits & 0x00000100u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      300, this->compress_type(), target);
  }

  // optional .hdcs.networking.CompressType expected_response_compress_type = 301;
  if (cached_has_bits & 0x00000008u) {
    target = ::google::protobuf::internal::WireFormatLite::WriteEnumToArray(
      301, this->expected_response_compress_type(), target);
  }

  if (_internal_metadata_.have_unknown_fields()) {
    target = ::google::protobuf::internal::WireFormat::SerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields(), target);
  }
  // @@protoc_insertion_point(serialize_to_array_end:hdcs.networking.Meta)
  return target;
}

size_t Meta::RequiredFieldsByteSizeFallback() const {
// @@protoc_insertion_point(required_fields_byte_size_fallback_start:hdcs.networking.Meta)
  size_t total_size = 0;

  if (has_sequence_id()) {
    // required uint64 sequence_id = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt64Size(
        this->sequence_id());
  }

  if (has_type()) {
    // required .hdcs.networking.Meta.Type type = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());
  }

  return total_size;
}
size_t Meta::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:hdcs.networking.Meta)
  size_t total_size = 0;

  if (_internal_metadata_.have_unknown_fields()) {
    total_size +=
      ::google::protobuf::internal::WireFormat::ComputeUnknownFieldsSize(
        _internal_metadata_.unknown_fields());
  }
  if (((_has_bits_[0] & 0x00000014) ^ 0x00000014) == 0) {  // All required fields are present.
    // required uint64 sequence_id = 2;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::UInt64Size(
        this->sequence_id());

    // required .hdcs.networking.Meta.Type type = 1;
    total_size += 1 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->type());

  } else {
    total_size += RequiredFieldsByteSizeFallback();
  }
  if (_has_bits_[0 / 32] & 3u) {
    // optional string method = 100;
    if (has_method()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->method());
    }

    // optional string reason = 202;
    if (has_reason()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::StringSize(
          this->reason());
    }

  }
  // optional .hdcs.networking.CompressType expected_response_compress_type = 301;
  if (has_expected_response_compress_type()) {
    total_size += 2 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->expected_response_compress_type());
  }

  if (_has_bits_[0 / 32] & 224u) {
    // optional bool failed = 200;
    if (has_failed()) {
      total_size += 2 + 1;
    }

    // optional int64 server_timeout = 101;
    if (has_server_timeout()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::Int64Size(
          this->server_timeout());
    }

    // optional int32 error_code = 201;
    if (has_error_code()) {
      total_size += 2 +
        ::google::protobuf::internal::WireFormatLite::Int32Size(
          this->error_code());
    }

  }
  // optional .hdcs.networking.CompressType compress_type = 300;
  if (has_compress_type()) {
    total_size += 2 +
      ::google::protobuf::internal::WireFormatLite::EnumSize(this->compress_type());
  }

  int cached_size = ::google::protobuf::internal::ToCachedSize(total_size);
  GOOGLE_SAFE_CONCURRENT_WRITES_BEGIN();
  _cached_size_ = cached_size;
  GOOGLE_SAFE_CONCURRENT_WRITES_END();
  return total_size;
}

void Meta::MergeFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_merge_from_start:hdcs.networking.Meta)
  GOOGLE_DCHECK_NE(&from, this);
  const Meta* source =
      ::google::protobuf::internal::DynamicCastToGenerated<const Meta>(
          &from);
  if (source == NULL) {
  // @@protoc_insertion_point(generalized_merge_from_cast_fail:hdcs.networking.Meta)
    ::google::protobuf::internal::ReflectionOps::Merge(from, this);
  } else {
  // @@protoc_insertion_point(generalized_merge_from_cast_success:hdcs.networking.Meta)
    MergeFrom(*source);
  }
}

void Meta::MergeFrom(const Meta& from) {
// @@protoc_insertion_point(class_specific_merge_from_start:hdcs.networking.Meta)
  GOOGLE_DCHECK_NE(&from, this);
  _internal_metadata_.MergeFrom(from._internal_metadata_);
  ::google::protobuf::uint32 cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._has_bits_[0];
  if (cached_has_bits & 255u) {
    if (cached_has_bits & 0x00000001u) {
      set_has_method();
      method_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.method_);
    }
    if (cached_has_bits & 0x00000002u) {
      set_has_reason();
      reason_.AssignWithDefault(&::google::protobuf::internal::GetEmptyStringAlreadyInited(), from.reason_);
    }
    if (cached_has_bits & 0x00000004u) {
      sequence_id_ = from.sequence_id_;
    }
    if (cached_has_bits & 0x00000008u) {
      expected_response_compress_type_ = from.expected_response_compress_type_;
    }
    if (cached_has_bits & 0x00000010u) {
      type_ = from.type_;
    }
    if (cached_has_bits & 0x00000020u) {
      failed_ = from.failed_;
    }
    if (cached_has_bits & 0x00000040u) {
      server_timeout_ = from.server_timeout_;
    }
    if (cached_has_bits & 0x00000080u) {
      error_code_ = from.error_code_;
    }
    _has_bits_[0] |= cached_has_bits;
  }
  if (cached_has_bits & 0x00000100u) {
    set_compress_type(from.compress_type());
  }
}

void Meta::CopyFrom(const ::google::protobuf::Message& from) {
// @@protoc_insertion_point(generalized_copy_from_start:hdcs.networking.Meta)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

void Meta::CopyFrom(const Meta& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:hdcs.networking.Meta)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool Meta::IsInitialized() const {
  if ((_has_bits_[0] & 0x00000014) != 0x00000014) return false;
  return true;
}

void Meta::Swap(Meta* other) {
  if (other == this) return;
  InternalSwap(other);
}
void Meta::InternalSwap(Meta* other) {
  using std::swap;
  method_.Swap(&other->method_);
  reason_.Swap(&other->reason_);
  swap(sequence_id_, other->sequence_id_);
  swap(expected_response_compress_type_, other->expected_response_compress_type_);
  swap(type_, other->type_);
  swap(failed_, other->failed_);
  swap(server_timeout_, other->server_timeout_);
  swap(error_code_, other->error_code_);
  swap(compress_type_, other->compress_type_);
  swap(_has_bits_[0], other->_has_bits_[0]);
  _internal_metadata_.Swap(&other->_internal_metadata_);
  swap(_cached_size_, other->_cached_size_);
}

::google::protobuf::Metadata Meta::GetMetadata() const {
  protobuf_meta_2eproto::protobuf_AssignDescriptorsOnce();
  return ::protobuf_meta_2eproto::file_level_metadata[kIndexInFileMessages];
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace networking
}  // namespace hdcs

// @@protoc_insertion_point(global_scope)
