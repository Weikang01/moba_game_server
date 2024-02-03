// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game.proto

#include "game.pb.h"

#include <algorithm>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>

PROTOBUF_PRAGMA_INIT_SEG

namespace _pb = ::PROTOBUF_NAMESPACE_ID;
namespace _pbi = _pb::internal;

PROTOBUF_CONSTEXPR LoginReq::LoginReq(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.username_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.email_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.age_)*/0
  , /*decltype(_impl_.int_set_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct LoginReqDefaultTypeInternal {
  PROTOBUF_CONSTEXPR LoginReqDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~LoginReqDefaultTypeInternal() {}
  union {
    LoginReq _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 LoginReqDefaultTypeInternal _LoginReq_default_instance_;
PROTOBUF_CONSTEXPR LoginRes::LoginRes(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.status_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct LoginResDefaultTypeInternal {
  PROTOBUF_CONSTEXPR LoginResDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~LoginResDefaultTypeInternal() {}
  union {
    LoginRes _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 LoginResDefaultTypeInternal _LoginRes_default_instance_;
static ::_pb::Metadata file_level_metadata_game_2eproto[2];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_game_2eproto[1];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_game_2eproto = nullptr;

const uint32_t TableStruct_game_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::LoginReq, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::LoginReq, _impl_.username_),
  PROTOBUF_FIELD_OFFSET(::LoginReq, _impl_.age_),
  PROTOBUF_FIELD_OFFSET(::LoginReq, _impl_.email_),
  PROTOBUF_FIELD_OFFSET(::LoginReq, _impl_.int_set_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::LoginRes, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::LoginRes, _impl_.status_),
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::LoginReq)},
  { 10, -1, -1, sizeof(::LoginRes)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::_LoginReq_default_instance_._instance,
  &::_LoginRes_default_instance_._instance,
};

const char descriptor_table_protodef_game_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\ngame.proto\"I\n\010LoginReq\022\020\n\010username\030\001 \001"
  "(\t\022\013\n\003age\030\002 \001(\005\022\r\n\005email\030\003 \001(\t\022\017\n\007int_se"
  "t\030\004 \001(\005\"\032\n\010LoginRes\022\016\n\006status\030\001 \001(\005*#\n\003C"
  "md\022\r\n\teLoginReq\020\000\022\r\n\teLoginRes\020\001b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_game_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_game_2eproto = {
    false, false, 160, descriptor_table_protodef_game_2eproto,
    "game.proto",
    &descriptor_table_game_2eproto_once, nullptr, 0, 2,
    schemas, file_default_instances, TableStruct_game_2eproto::offsets,
    file_level_metadata_game_2eproto, file_level_enum_descriptors_game_2eproto,
    file_level_service_descriptors_game_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_game_2eproto_getter() {
  return &descriptor_table_game_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_game_2eproto(&descriptor_table_game_2eproto);
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Cmd_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_game_2eproto);
  return file_level_enum_descriptors_game_2eproto[0];
}
bool Cmd_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class LoginReq::_Internal {
 public:
};

LoginReq::LoginReq(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:LoginReq)
}
LoginReq::LoginReq(const LoginReq& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  LoginReq* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.username_){}
    , decltype(_impl_.email_){}
    , decltype(_impl_.age_){}
    , decltype(_impl_.int_set_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.username_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.username_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_username().empty()) {
    _this->_impl_.username_.Set(from._internal_username(), 
      _this->GetArenaForAllocation());
  }
  _impl_.email_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.email_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_email().empty()) {
    _this->_impl_.email_.Set(from._internal_email(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.age_, &from._impl_.age_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.int_set_) -
    reinterpret_cast<char*>(&_impl_.age_)) + sizeof(_impl_.int_set_));
  // @@protoc_insertion_point(copy_constructor:LoginReq)
}

inline void LoginReq::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.username_){}
    , decltype(_impl_.email_){}
    , decltype(_impl_.age_){0}
    , decltype(_impl_.int_set_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.username_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.username_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  _impl_.email_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.email_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

LoginReq::~LoginReq() {
  // @@protoc_insertion_point(destructor:LoginReq)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void LoginReq::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.username_.Destroy();
  _impl_.email_.Destroy();
}

void LoginReq::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void LoginReq::Clear() {
// @@protoc_insertion_point(message_clear_start:LoginReq)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.username_.ClearToEmpty();
  _impl_.email_.ClearToEmpty();
  ::memset(&_impl_.age_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.int_set_) -
      reinterpret_cast<char*>(&_impl_.age_)) + sizeof(_impl_.int_set_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* LoginReq::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string username = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_username();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "LoginReq.username"));
        } else
          goto handle_unusual;
        continue;
      // int32 age = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.age_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // string email = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 26)) {
          auto str = _internal_mutable_email();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "LoginReq.email"));
        } else
          goto handle_unusual;
        continue;
      // int32 int_set = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          _impl_.int_set_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* LoginReq::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:LoginReq)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string username = 1;
  if (!this->_internal_username().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_username().data(), static_cast<int>(this->_internal_username().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "LoginReq.username");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_username(), target);
  }

  // int32 age = 2;
  if (this->_internal_age() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(2, this->_internal_age(), target);
  }

  // string email = 3;
  if (!this->_internal_email().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_email().data(), static_cast<int>(this->_internal_email().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "LoginReq.email");
    target = stream->WriteStringMaybeAliased(
        3, this->_internal_email(), target);
  }

  // int32 int_set = 4;
  if (this->_internal_int_set() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(4, this->_internal_int_set(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:LoginReq)
  return target;
}

size_t LoginReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:LoginReq)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string username = 1;
  if (!this->_internal_username().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_username());
  }

  // string email = 3;
  if (!this->_internal_email().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_email());
  }

  // int32 age = 2;
  if (this->_internal_age() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_age());
  }

  // int32 int_set = 4;
  if (this->_internal_int_set() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_int_set());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData LoginReq::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    LoginReq::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*LoginReq::GetClassData() const { return &_class_data_; }


void LoginReq::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<LoginReq*>(&to_msg);
  auto& from = static_cast<const LoginReq&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:LoginReq)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_username().empty()) {
    _this->_internal_set_username(from._internal_username());
  }
  if (!from._internal_email().empty()) {
    _this->_internal_set_email(from._internal_email());
  }
  if (from._internal_age() != 0) {
    _this->_internal_set_age(from._internal_age());
  }
  if (from._internal_int_set() != 0) {
    _this->_internal_set_int_set(from._internal_int_set());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void LoginReq::CopyFrom(const LoginReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:LoginReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LoginReq::IsInitialized() const {
  return true;
}

void LoginReq::InternalSwap(LoginReq* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.username_, lhs_arena,
      &other->_impl_.username_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.email_, lhs_arena,
      &other->_impl_.email_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(LoginReq, _impl_.int_set_)
      + sizeof(LoginReq::_impl_.int_set_)
      - PROTOBUF_FIELD_OFFSET(LoginReq, _impl_.age_)>(
          reinterpret_cast<char*>(&_impl_.age_),
          reinterpret_cast<char*>(&other->_impl_.age_));
}

::PROTOBUF_NAMESPACE_ID::Metadata LoginReq::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_game_2eproto_getter, &descriptor_table_game_2eproto_once,
      file_level_metadata_game_2eproto[0]);
}

// ===================================================================

class LoginRes::_Internal {
 public:
};

LoginRes::LoginRes(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:LoginRes)
}
LoginRes::LoginRes(const LoginRes& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  LoginRes* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.status_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _this->_impl_.status_ = from._impl_.status_;
  // @@protoc_insertion_point(copy_constructor:LoginRes)
}

inline void LoginRes::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.status_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
}

LoginRes::~LoginRes() {
  // @@protoc_insertion_point(destructor:LoginRes)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void LoginRes::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
}

void LoginRes::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void LoginRes::Clear() {
// @@protoc_insertion_point(message_clear_start:LoginRes)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.status_ = 0;
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* LoginRes::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // int32 status = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 8)) {
          _impl_.status_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      default:
        goto handle_unusual;
    }  // switch
  handle_unusual:
    if ((tag == 0) || ((tag & 7) == 4)) {
      CHK_(ptr);
      ctx->SetLastTag(tag);
      goto message_done;
    }
    ptr = UnknownFieldParse(
        tag,
        _internal_metadata_.mutable_unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(),
        ptr, ctx);
    CHK_(ptr != nullptr);
  }  // while
message_done:
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* LoginRes::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:LoginRes)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 status = 1;
  if (this->_internal_status() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_status(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:LoginRes)
  return target;
}

size_t LoginRes::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:LoginRes)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // int32 status = 1;
  if (this->_internal_status() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_status());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData LoginRes::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    LoginRes::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*LoginRes::GetClassData() const { return &_class_data_; }


void LoginRes::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<LoginRes*>(&to_msg);
  auto& from = static_cast<const LoginRes&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:LoginRes)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_status() != 0) {
    _this->_internal_set_status(from._internal_status());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void LoginRes::CopyFrom(const LoginRes& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:LoginRes)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool LoginRes::IsInitialized() const {
  return true;
}

void LoginRes::InternalSwap(LoginRes* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_.status_, other->_impl_.status_);
}

::PROTOBUF_NAMESPACE_ID::Metadata LoginRes::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_game_2eproto_getter, &descriptor_table_game_2eproto_once,
      file_level_metadata_game_2eproto[1]);
}

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::LoginReq*
Arena::CreateMaybeMessage< ::LoginReq >(Arena* arena) {
  return Arena::CreateMessageInternal< ::LoginReq >(arena);
}
template<> PROTOBUF_NOINLINE ::LoginRes*
Arena::CreateMaybeMessage< ::LoginRes >(Arena* arena) {
  return Arena::CreateMessageInternal< ::LoginRes >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
