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

PROTOBUF_CONSTEXPR GuestLoginReq::GuestLoginReq(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.guest_key_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct GuestLoginReqDefaultTypeInternal {
  PROTOBUF_CONSTEXPR GuestLoginReqDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~GuestLoginReqDefaultTypeInternal() {}
  union {
    GuestLoginReq _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 GuestLoginReqDefaultTypeInternal _GuestLoginReq_default_instance_;
PROTOBUF_CONSTEXPR UserCenterInfo::UserCenterInfo(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_.unick_)*/{&::_pbi::fixed_address_empty_string, ::_pbi::ConstantInitialized{}}
  , /*decltype(_impl_.usex_)*/0
  , /*decltype(_impl_.usysavatar_)*/0
  , /*decltype(_impl_.uvip_)*/0
  , /*decltype(_impl_.uid_)*/0
  , /*decltype(_impl_._cached_size_)*/{}} {}
struct UserCenterInfoDefaultTypeInternal {
  PROTOBUF_CONSTEXPR UserCenterInfoDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~UserCenterInfoDefaultTypeInternal() {}
  union {
    UserCenterInfo _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 UserCenterInfoDefaultTypeInternal _UserCenterInfo_default_instance_;
PROTOBUF_CONSTEXPR GuestLoginRes::GuestLoginRes(
    ::_pbi::ConstantInitialized): _impl_{
    /*decltype(_impl_._has_bits_)*/{}
  , /*decltype(_impl_._cached_size_)*/{}
  , /*decltype(_impl_.uinfo_)*/nullptr
  , /*decltype(_impl_.status_)*/0} {}
struct GuestLoginResDefaultTypeInternal {
  PROTOBUF_CONSTEXPR GuestLoginResDefaultTypeInternal()
      : _instance(::_pbi::ConstantInitialized{}) {}
  ~GuestLoginResDefaultTypeInternal() {}
  union {
    GuestLoginRes _instance;
  };
};
PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 GuestLoginResDefaultTypeInternal _GuestLoginRes_default_instance_;
static ::_pb::Metadata file_level_metadata_game_2eproto[3];
static const ::_pb::EnumDescriptor* file_level_enum_descriptors_game_2eproto[2];
static constexpr ::_pb::ServiceDescriptor const** file_level_service_descriptors_game_2eproto = nullptr;

const uint32_t TableStruct_game_2eproto::offsets[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::GuestLoginReq, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::GuestLoginReq, _impl_.guest_key_),
  ~0u,  // no _has_bits_
  PROTOBUF_FIELD_OFFSET(::UserCenterInfo, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::UserCenterInfo, _impl_.unick_),
  PROTOBUF_FIELD_OFFSET(::UserCenterInfo, _impl_.usex_),
  PROTOBUF_FIELD_OFFSET(::UserCenterInfo, _impl_.usysavatar_),
  PROTOBUF_FIELD_OFFSET(::UserCenterInfo, _impl_.uvip_),
  PROTOBUF_FIELD_OFFSET(::UserCenterInfo, _impl_.uid_),
  PROTOBUF_FIELD_OFFSET(::GuestLoginRes, _impl_._has_bits_),
  PROTOBUF_FIELD_OFFSET(::GuestLoginRes, _internal_metadata_),
  ~0u,  // no _extensions_
  ~0u,  // no _oneof_case_
  ~0u,  // no _weak_field_map_
  ~0u,  // no _inlined_string_donated_
  PROTOBUF_FIELD_OFFSET(::GuestLoginRes, _impl_.status_),
  PROTOBUF_FIELD_OFFSET(::GuestLoginRes, _impl_.uinfo_),
  ~0u,
  0,
};
static const ::_pbi::MigrationSchema schemas[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) = {
  { 0, -1, -1, sizeof(::GuestLoginReq)},
  { 7, -1, -1, sizeof(::UserCenterInfo)},
  { 18, 26, -1, sizeof(::GuestLoginRes)},
};

static const ::_pb::Message* const file_default_instances[] = {
  &::_GuestLoginReq_default_instance_._instance,
  &::_UserCenterInfo_default_instance_._instance,
  &::_GuestLoginRes_default_instance_._instance,
};

const char descriptor_table_protodef_game_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\ngame.proto\"\"\n\rGuestLoginReq\022\021\n\tguest_k"
  "ey\030\001 \001(\t\"\\\n\016UserCenterInfo\022\r\n\005unick\030\001 \001("
  "\t\022\014\n\004usex\030\002 \001(\005\022\022\n\nusysavatar\030\003 \001(\005\022\014\n\004u"
  "vip\030\004 \001(\005\022\013\n\003uid\030\005 \001(\005\"N\n\rGuestLoginRes\022"
  "\016\n\006status\030\001 \001(\005\022#\n\005uinfo\030\002 \001(\0132\017.UserCen"
  "terInfoH\000\210\001\001B\010\n\006_uinfo*>\n\005Stype\022\021\n\rINVAL"
  "ID_STYPE\020\000\022\t\n\005eAuth\020\001\022\013\n\007eSystem\020\002\022\n\n\006eL"
  "ogic\020\003*>\n\003Cmd\022\017\n\013INVALID_CMD\020\000\022\022\n\016eGuest"
  "LoginReq\020\001\022\022\n\016eGuestLoginRes\020\002b\006proto3"
  ;
static ::_pbi::once_flag descriptor_table_game_2eproto_once;
const ::_pbi::DescriptorTable descriptor_table_game_2eproto = {
    false, false, 358, descriptor_table_protodef_game_2eproto,
    "game.proto",
    &descriptor_table_game_2eproto_once, nullptr, 0, 3,
    schemas, file_default_instances, TableStruct_game_2eproto::offsets,
    file_level_metadata_game_2eproto, file_level_enum_descriptors_game_2eproto,
    file_level_service_descriptors_game_2eproto,
};
PROTOBUF_ATTRIBUTE_WEAK const ::_pbi::DescriptorTable* descriptor_table_game_2eproto_getter() {
  return &descriptor_table_game_2eproto;
}

// Force running AddDescriptors() at dynamic initialization time.
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::_pbi::AddDescriptorsRunner dynamic_init_dummy_game_2eproto(&descriptor_table_game_2eproto);
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Stype_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_game_2eproto);
  return file_level_enum_descriptors_game_2eproto[0];
}
bool Stype_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
      return true;
    default:
      return false;
  }
}

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Cmd_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_game_2eproto);
  return file_level_enum_descriptors_game_2eproto[1];
}
bool Cmd_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
      return true;
    default:
      return false;
  }
}


// ===================================================================

class GuestLoginReq::_Internal {
 public:
};

GuestLoginReq::GuestLoginReq(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:GuestLoginReq)
}
GuestLoginReq::GuestLoginReq(const GuestLoginReq& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  GuestLoginReq* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.guest_key_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.guest_key_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.guest_key_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_guest_key().empty()) {
    _this->_impl_.guest_key_.Set(from._internal_guest_key(), 
      _this->GetArenaForAllocation());
  }
  // @@protoc_insertion_point(copy_constructor:GuestLoginReq)
}

inline void GuestLoginReq::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.guest_key_){}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.guest_key_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.guest_key_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

GuestLoginReq::~GuestLoginReq() {
  // @@protoc_insertion_point(destructor:GuestLoginReq)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void GuestLoginReq::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.guest_key_.Destroy();
}

void GuestLoginReq::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void GuestLoginReq::Clear() {
// @@protoc_insertion_point(message_clear_start:GuestLoginReq)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.guest_key_.ClearToEmpty();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* GuestLoginReq::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string guest_key = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_guest_key();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "GuestLoginReq.guest_key"));
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

uint8_t* GuestLoginReq::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:GuestLoginReq)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string guest_key = 1;
  if (!this->_internal_guest_key().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_guest_key().data(), static_cast<int>(this->_internal_guest_key().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "GuestLoginReq.guest_key");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_guest_key(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:GuestLoginReq)
  return target;
}

size_t GuestLoginReq::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:GuestLoginReq)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string guest_key = 1;
  if (!this->_internal_guest_key().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_guest_key());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData GuestLoginReq::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    GuestLoginReq::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GuestLoginReq::GetClassData() const { return &_class_data_; }


void GuestLoginReq::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<GuestLoginReq*>(&to_msg);
  auto& from = static_cast<const GuestLoginReq&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:GuestLoginReq)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_guest_key().empty()) {
    _this->_internal_set_guest_key(from._internal_guest_key());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void GuestLoginReq::CopyFrom(const GuestLoginReq& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:GuestLoginReq)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GuestLoginReq::IsInitialized() const {
  return true;
}

void GuestLoginReq::InternalSwap(GuestLoginReq* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.guest_key_, lhs_arena,
      &other->_impl_.guest_key_, rhs_arena
  );
}

::PROTOBUF_NAMESPACE_ID::Metadata GuestLoginReq::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_game_2eproto_getter, &descriptor_table_game_2eproto_once,
      file_level_metadata_game_2eproto[0]);
}

// ===================================================================

class UserCenterInfo::_Internal {
 public:
};

UserCenterInfo::UserCenterInfo(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:UserCenterInfo)
}
UserCenterInfo::UserCenterInfo(const UserCenterInfo& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  UserCenterInfo* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_.unick_){}
    , decltype(_impl_.usex_){}
    , decltype(_impl_.usysavatar_){}
    , decltype(_impl_.uvip_){}
    , decltype(_impl_.uid_){}
    , /*decltype(_impl_._cached_size_)*/{}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  _impl_.unick_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.unick_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (!from._internal_unick().empty()) {
    _this->_impl_.unick_.Set(from._internal_unick(), 
      _this->GetArenaForAllocation());
  }
  ::memcpy(&_impl_.usex_, &from._impl_.usex_,
    static_cast<size_t>(reinterpret_cast<char*>(&_impl_.uid_) -
    reinterpret_cast<char*>(&_impl_.usex_)) + sizeof(_impl_.uid_));
  // @@protoc_insertion_point(copy_constructor:UserCenterInfo)
}

inline void UserCenterInfo::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_.unick_){}
    , decltype(_impl_.usex_){0}
    , decltype(_impl_.usysavatar_){0}
    , decltype(_impl_.uvip_){0}
    , decltype(_impl_.uid_){0}
    , /*decltype(_impl_._cached_size_)*/{}
  };
  _impl_.unick_.InitDefault();
  #ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
    _impl_.unick_.Set("", GetArenaForAllocation());
  #endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
}

UserCenterInfo::~UserCenterInfo() {
  // @@protoc_insertion_point(destructor:UserCenterInfo)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void UserCenterInfo::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  _impl_.unick_.Destroy();
}

void UserCenterInfo::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void UserCenterInfo::Clear() {
// @@protoc_insertion_point(message_clear_start:UserCenterInfo)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  _impl_.unick_.ClearToEmpty();
  ::memset(&_impl_.usex_, 0, static_cast<size_t>(
      reinterpret_cast<char*>(&_impl_.uid_) -
      reinterpret_cast<char*>(&_impl_.usex_)) + sizeof(_impl_.uid_));
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* UserCenterInfo::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  while (!ctx->Done(&ptr)) {
    uint32_t tag;
    ptr = ::_pbi::ReadTag(ptr, &tag);
    switch (tag >> 3) {
      // string unick = 1;
      case 1:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 10)) {
          auto str = _internal_mutable_unick();
          ptr = ::_pbi::InlineGreedyStringParser(str, ptr, ctx);
          CHK_(ptr);
          CHK_(::_pbi::VerifyUTF8(str, "UserCenterInfo.unick"));
        } else
          goto handle_unusual;
        continue;
      // int32 usex = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 16)) {
          _impl_.usex_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 usysavatar = 3;
      case 3:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 24)) {
          _impl_.usysavatar_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 uvip = 4;
      case 4:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 32)) {
          _impl_.uvip_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
          CHK_(ptr);
        } else
          goto handle_unusual;
        continue;
      // int32 uid = 5;
      case 5:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 40)) {
          _impl_.uid_ = ::PROTOBUF_NAMESPACE_ID::internal::ReadVarint32(&ptr);
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

uint8_t* UserCenterInfo::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:UserCenterInfo)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // string unick = 1;
  if (!this->_internal_unick().empty()) {
    ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::VerifyUtf8String(
      this->_internal_unick().data(), static_cast<int>(this->_internal_unick().length()),
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::SERIALIZE,
      "UserCenterInfo.unick");
    target = stream->WriteStringMaybeAliased(
        1, this->_internal_unick(), target);
  }

  // int32 usex = 2;
  if (this->_internal_usex() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(2, this->_internal_usex(), target);
  }

  // int32 usysavatar = 3;
  if (this->_internal_usysavatar() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(3, this->_internal_usysavatar(), target);
  }

  // int32 uvip = 4;
  if (this->_internal_uvip() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(4, this->_internal_uvip(), target);
  }

  // int32 uid = 5;
  if (this->_internal_uid() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(5, this->_internal_uid(), target);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:UserCenterInfo)
  return target;
}

size_t UserCenterInfo::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:UserCenterInfo)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // string unick = 1;
  if (!this->_internal_unick().empty()) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::StringSize(
        this->_internal_unick());
  }

  // int32 usex = 2;
  if (this->_internal_usex() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_usex());
  }

  // int32 usysavatar = 3;
  if (this->_internal_usysavatar() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_usysavatar());
  }

  // int32 uvip = 4;
  if (this->_internal_uvip() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_uvip());
  }

  // int32 uid = 5;
  if (this->_internal_uid() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_uid());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData UserCenterInfo::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    UserCenterInfo::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*UserCenterInfo::GetClassData() const { return &_class_data_; }


void UserCenterInfo::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<UserCenterInfo*>(&to_msg);
  auto& from = static_cast<const UserCenterInfo&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:UserCenterInfo)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (!from._internal_unick().empty()) {
    _this->_internal_set_unick(from._internal_unick());
  }
  if (from._internal_usex() != 0) {
    _this->_internal_set_usex(from._internal_usex());
  }
  if (from._internal_usysavatar() != 0) {
    _this->_internal_set_usysavatar(from._internal_usysavatar());
  }
  if (from._internal_uvip() != 0) {
    _this->_internal_set_uvip(from._internal_uvip());
  }
  if (from._internal_uid() != 0) {
    _this->_internal_set_uid(from._internal_uid());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void UserCenterInfo::CopyFrom(const UserCenterInfo& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:UserCenterInfo)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool UserCenterInfo::IsInitialized() const {
  return true;
}

void UserCenterInfo::InternalSwap(UserCenterInfo* other) {
  using std::swap;
  auto* lhs_arena = GetArenaForAllocation();
  auto* rhs_arena = other->GetArenaForAllocation();
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr::InternalSwap(
      &_impl_.unick_, lhs_arena,
      &other->_impl_.unick_, rhs_arena
  );
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(UserCenterInfo, _impl_.uid_)
      + sizeof(UserCenterInfo::_impl_.uid_)
      - PROTOBUF_FIELD_OFFSET(UserCenterInfo, _impl_.usex_)>(
          reinterpret_cast<char*>(&_impl_.usex_),
          reinterpret_cast<char*>(&other->_impl_.usex_));
}

::PROTOBUF_NAMESPACE_ID::Metadata UserCenterInfo::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_game_2eproto_getter, &descriptor_table_game_2eproto_once,
      file_level_metadata_game_2eproto[1]);
}

// ===================================================================

class GuestLoginRes::_Internal {
 public:
  using HasBits = decltype(std::declval<GuestLoginRes>()._impl_._has_bits_);
  static const ::UserCenterInfo& uinfo(const GuestLoginRes* msg);
  static void set_has_uinfo(HasBits* has_bits) {
    (*has_bits)[0] |= 1u;
  }
};

const ::UserCenterInfo&
GuestLoginRes::_Internal::uinfo(const GuestLoginRes* msg) {
  return *msg->_impl_.uinfo_;
}
GuestLoginRes::GuestLoginRes(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                         bool is_message_owned)
  : ::PROTOBUF_NAMESPACE_ID::Message(arena, is_message_owned) {
  SharedCtor(arena, is_message_owned);
  // @@protoc_insertion_point(arena_constructor:GuestLoginRes)
}
GuestLoginRes::GuestLoginRes(const GuestLoginRes& from)
  : ::PROTOBUF_NAMESPACE_ID::Message() {
  GuestLoginRes* const _this = this; (void)_this;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){from._impl_._has_bits_}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.uinfo_){nullptr}
    , decltype(_impl_.status_){}};

  _internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
  if (from._internal_has_uinfo()) {
    _this->_impl_.uinfo_ = new ::UserCenterInfo(*from._impl_.uinfo_);
  }
  _this->_impl_.status_ = from._impl_.status_;
  // @@protoc_insertion_point(copy_constructor:GuestLoginRes)
}

inline void GuestLoginRes::SharedCtor(
    ::_pb::Arena* arena, bool is_message_owned) {
  (void)arena;
  (void)is_message_owned;
  new (&_impl_) Impl_{
      decltype(_impl_._has_bits_){}
    , /*decltype(_impl_._cached_size_)*/{}
    , decltype(_impl_.uinfo_){nullptr}
    , decltype(_impl_.status_){0}
  };
}

GuestLoginRes::~GuestLoginRes() {
  // @@protoc_insertion_point(destructor:GuestLoginRes)
  if (auto *arena = _internal_metadata_.DeleteReturnArena<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>()) {
  (void)arena;
    return;
  }
  SharedDtor();
}

inline void GuestLoginRes::SharedDtor() {
  GOOGLE_DCHECK(GetArenaForAllocation() == nullptr);
  if (this != internal_default_instance()) delete _impl_.uinfo_;
}

void GuestLoginRes::SetCachedSize(int size) const {
  _impl_._cached_size_.Set(size);
}

void GuestLoginRes::Clear() {
// @@protoc_insertion_point(message_clear_start:GuestLoginRes)
  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    GOOGLE_DCHECK(_impl_.uinfo_ != nullptr);
    _impl_.uinfo_->Clear();
  }
  _impl_.status_ = 0;
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>();
}

const char* GuestLoginRes::_InternalParse(const char* ptr, ::_pbi::ParseContext* ctx) {
#define CHK_(x) if (PROTOBUF_PREDICT_FALSE(!(x))) goto failure
  _Internal::HasBits has_bits{};
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
      // optional .UserCenterInfo uinfo = 2;
      case 2:
        if (PROTOBUF_PREDICT_TRUE(static_cast<uint8_t>(tag) == 18)) {
          ptr = ctx->ParseMessage(_internal_mutable_uinfo(), ptr);
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
  _impl_._has_bits_.Or(has_bits);
  return ptr;
failure:
  ptr = nullptr;
  goto message_done;
#undef CHK_
}

uint8_t* GuestLoginRes::_InternalSerialize(
    uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const {
  // @@protoc_insertion_point(serialize_to_array_start:GuestLoginRes)
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  // int32 status = 1;
  if (this->_internal_status() != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteInt32ToArray(1, this->_internal_status(), target);
  }

  // optional .UserCenterInfo uinfo = 2;
  if (_internal_has_uinfo()) {
    target = ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::
      InternalWriteMessage(2, _Internal::uinfo(this),
        _Internal::uinfo(this).GetCachedSize(), target, stream);
  }

  if (PROTOBUF_PREDICT_FALSE(_internal_metadata_.have_unknown_fields())) {
    target = ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
        _internal_metadata_.unknown_fields<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(::PROTOBUF_NAMESPACE_ID::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:GuestLoginRes)
  return target;
}

size_t GuestLoginRes::ByteSizeLong() const {
// @@protoc_insertion_point(message_byte_size_start:GuestLoginRes)
  size_t total_size = 0;

  uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  // optional .UserCenterInfo uinfo = 2;
  cached_has_bits = _impl_._has_bits_[0];
  if (cached_has_bits & 0x00000001u) {
    total_size += 1 +
      ::PROTOBUF_NAMESPACE_ID::internal::WireFormatLite::MessageSize(
        *_impl_.uinfo_);
  }

  // int32 status = 1;
  if (this->_internal_status() != 0) {
    total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(this->_internal_status());
  }

  return MaybeComputeUnknownFieldsSize(total_size, &_impl_._cached_size_);
}

const ::PROTOBUF_NAMESPACE_ID::Message::ClassData GuestLoginRes::_class_data_ = {
    ::PROTOBUF_NAMESPACE_ID::Message::CopyWithSourceCheck,
    GuestLoginRes::MergeImpl
};
const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GuestLoginRes::GetClassData() const { return &_class_data_; }


void GuestLoginRes::MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg) {
  auto* const _this = static_cast<GuestLoginRes*>(&to_msg);
  auto& from = static_cast<const GuestLoginRes&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:GuestLoginRes)
  GOOGLE_DCHECK_NE(&from, _this);
  uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  if (from._internal_has_uinfo()) {
    _this->_internal_mutable_uinfo()->::UserCenterInfo::MergeFrom(
        from._internal_uinfo());
  }
  if (from._internal_status() != 0) {
    _this->_internal_set_status(from._internal_status());
  }
  _this->_internal_metadata_.MergeFrom<::PROTOBUF_NAMESPACE_ID::UnknownFieldSet>(from._internal_metadata_);
}

void GuestLoginRes::CopyFrom(const GuestLoginRes& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:GuestLoginRes)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

bool GuestLoginRes::IsInitialized() const {
  return true;
}

void GuestLoginRes::InternalSwap(GuestLoginRes* other) {
  using std::swap;
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::PROTOBUF_NAMESPACE_ID::internal::memswap<
      PROTOBUF_FIELD_OFFSET(GuestLoginRes, _impl_.status_)
      + sizeof(GuestLoginRes::_impl_.status_)
      - PROTOBUF_FIELD_OFFSET(GuestLoginRes, _impl_.uinfo_)>(
          reinterpret_cast<char*>(&_impl_.uinfo_),
          reinterpret_cast<char*>(&other->_impl_.uinfo_));
}

::PROTOBUF_NAMESPACE_ID::Metadata GuestLoginRes::GetMetadata() const {
  return ::_pbi::AssignDescriptors(
      &descriptor_table_game_2eproto_getter, &descriptor_table_game_2eproto_once,
      file_level_metadata_game_2eproto[2]);
}

// @@protoc_insertion_point(namespace_scope)
PROTOBUF_NAMESPACE_OPEN
template<> PROTOBUF_NOINLINE ::GuestLoginReq*
Arena::CreateMaybeMessage< ::GuestLoginReq >(Arena* arena) {
  return Arena::CreateMessageInternal< ::GuestLoginReq >(arena);
}
template<> PROTOBUF_NOINLINE ::UserCenterInfo*
Arena::CreateMaybeMessage< ::UserCenterInfo >(Arena* arena) {
  return Arena::CreateMessageInternal< ::UserCenterInfo >(arena);
}
template<> PROTOBUF_NOINLINE ::GuestLoginRes*
Arena::CreateMaybeMessage< ::GuestLoginRes >(Arena* arena) {
  return Arena::CreateMessageInternal< ::GuestLoginRes >(arena);
}
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
