// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: game.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_game_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_game_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3021000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3021012 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/metadata_lite.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_game_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_game_2eproto {
  static const uint32_t offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_game_2eproto;
class LoginReq;
struct LoginReqDefaultTypeInternal;
extern LoginReqDefaultTypeInternal _LoginReq_default_instance_;
class LoginRes;
struct LoginResDefaultTypeInternal;
extern LoginResDefaultTypeInternal _LoginRes_default_instance_;
PROTOBUF_NAMESPACE_OPEN
template<> ::LoginReq* Arena::CreateMaybeMessage<::LoginReq>(Arena*);
template<> ::LoginRes* Arena::CreateMaybeMessage<::LoginRes>(Arena*);
PROTOBUF_NAMESPACE_CLOSE

enum Cmd : int {
  INVALID_CMD = 0,
  eLoginReq = 1,
  eLoginRes = 2,
  Cmd_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::min(),
  Cmd_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<int32_t>::max()
};
bool Cmd_IsValid(int value);
constexpr Cmd Cmd_MIN = INVALID_CMD;
constexpr Cmd Cmd_MAX = eLoginRes;
constexpr int Cmd_ARRAYSIZE = Cmd_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* Cmd_descriptor();
template<typename T>
inline const std::string& Cmd_Name(T enum_t_value) {
  static_assert(::std::is_same<T, Cmd>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function Cmd_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    Cmd_descriptor(), enum_t_value);
}
inline bool Cmd_Parse(
    ::PROTOBUF_NAMESPACE_ID::ConstStringParam name, Cmd* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<Cmd>(
    Cmd_descriptor(), name, value);
}
// ===================================================================

class LoginReq final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:LoginReq) */ {
 public:
  inline LoginReq() : LoginReq(nullptr) {}
  ~LoginReq() override;
  explicit PROTOBUF_CONSTEXPR LoginReq(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  LoginReq(const LoginReq& from);
  LoginReq(LoginReq&& from) noexcept
    : LoginReq() {
    *this = ::std::move(from);
  }

  inline LoginReq& operator=(const LoginReq& from) {
    CopyFrom(from);
    return *this;
  }
  inline LoginReq& operator=(LoginReq&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const LoginReq& default_instance() {
    return *internal_default_instance();
  }
  static inline const LoginReq* internal_default_instance() {
    return reinterpret_cast<const LoginReq*>(
               &_LoginReq_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    0;

  friend void swap(LoginReq& a, LoginReq& b) {
    a.Swap(&b);
  }
  inline void Swap(LoginReq* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LoginReq* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  LoginReq* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<LoginReq>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const LoginReq& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const LoginReq& from) {
    LoginReq::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LoginReq* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "LoginReq";
  }
  protected:
  explicit LoginReq(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kUsernameFieldNumber = 1,
    kEmailFieldNumber = 3,
    kAgeFieldNumber = 2,
    kIntSetFieldNumber = 4,
  };
  // string username = 1;
  void clear_username();
  const std::string& username() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_username(ArgT0&& arg0, ArgT... args);
  std::string* mutable_username();
  PROTOBUF_NODISCARD std::string* release_username();
  void set_allocated_username(std::string* username);
  private:
  const std::string& _internal_username() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_username(const std::string& value);
  std::string* _internal_mutable_username();
  public:

  // string email = 3;
  void clear_email();
  const std::string& email() const;
  template <typename ArgT0 = const std::string&, typename... ArgT>
  void set_email(ArgT0&& arg0, ArgT... args);
  std::string* mutable_email();
  PROTOBUF_NODISCARD std::string* release_email();
  void set_allocated_email(std::string* email);
  private:
  const std::string& _internal_email() const;
  inline PROTOBUF_ALWAYS_INLINE void _internal_set_email(const std::string& value);
  std::string* _internal_mutable_email();
  public:

  // int32 age = 2;
  void clear_age();
  int32_t age() const;
  void set_age(int32_t value);
  private:
  int32_t _internal_age() const;
  void _internal_set_age(int32_t value);
  public:

  // int32 int_set = 4;
  void clear_int_set();
  int32_t int_set() const;
  void set_int_set(int32_t value);
  private:
  int32_t _internal_int_set() const;
  void _internal_set_int_set(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:LoginReq)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr username_;
    ::PROTOBUF_NAMESPACE_ID::internal::ArenaStringPtr email_;
    int32_t age_;
    int32_t int_set_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_game_2eproto;
};
// -------------------------------------------------------------------

class LoginRes final :
    public ::PROTOBUF_NAMESPACE_ID::Message /* @@protoc_insertion_point(class_definition:LoginRes) */ {
 public:
  inline LoginRes() : LoginRes(nullptr) {}
  ~LoginRes() override;
  explicit PROTOBUF_CONSTEXPR LoginRes(::PROTOBUF_NAMESPACE_ID::internal::ConstantInitialized);

  LoginRes(const LoginRes& from);
  LoginRes(LoginRes&& from) noexcept
    : LoginRes() {
    *this = ::std::move(from);
  }

  inline LoginRes& operator=(const LoginRes& from) {
    CopyFrom(from);
    return *this;
  }
  inline LoginRes& operator=(LoginRes&& from) noexcept {
    if (this == &from) return *this;
    if (GetOwningArena() == from.GetOwningArena()
  #ifdef PROTOBUF_FORCE_COPY_IN_MOVE
        && GetOwningArena() != nullptr
  #endif  // !PROTOBUF_FORCE_COPY_IN_MOVE
    ) {
      InternalSwap(&from);
    } else {
      CopyFrom(from);
    }
    return *this;
  }

  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* descriptor() {
    return GetDescriptor();
  }
  static const ::PROTOBUF_NAMESPACE_ID::Descriptor* GetDescriptor() {
    return default_instance().GetMetadata().descriptor;
  }
  static const ::PROTOBUF_NAMESPACE_ID::Reflection* GetReflection() {
    return default_instance().GetMetadata().reflection;
  }
  static const LoginRes& default_instance() {
    return *internal_default_instance();
  }
  static inline const LoginRes* internal_default_instance() {
    return reinterpret_cast<const LoginRes*>(
               &_LoginRes_default_instance_);
  }
  static constexpr int kIndexInFileMessages =
    1;

  friend void swap(LoginRes& a, LoginRes& b) {
    a.Swap(&b);
  }
  inline void Swap(LoginRes* other) {
    if (other == this) return;
  #ifdef PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() != nullptr &&
        GetOwningArena() == other->GetOwningArena()) {
   #else  // PROTOBUF_FORCE_COPY_IN_SWAP
    if (GetOwningArena() == other->GetOwningArena()) {
  #endif  // !PROTOBUF_FORCE_COPY_IN_SWAP
      InternalSwap(other);
    } else {
      ::PROTOBUF_NAMESPACE_ID::internal::GenericSwap(this, other);
    }
  }
  void UnsafeArenaSwap(LoginRes* other) {
    if (other == this) return;
    GOOGLE_DCHECK(GetOwningArena() == other->GetOwningArena());
    InternalSwap(other);
  }

  // implements Message ----------------------------------------------

  LoginRes* New(::PROTOBUF_NAMESPACE_ID::Arena* arena = nullptr) const final {
    return CreateMaybeMessage<LoginRes>(arena);
  }
  using ::PROTOBUF_NAMESPACE_ID::Message::CopyFrom;
  void CopyFrom(const LoginRes& from);
  using ::PROTOBUF_NAMESPACE_ID::Message::MergeFrom;
  void MergeFrom( const LoginRes& from) {
    LoginRes::MergeImpl(*this, from);
  }
  private:
  static void MergeImpl(::PROTOBUF_NAMESPACE_ID::Message& to_msg, const ::PROTOBUF_NAMESPACE_ID::Message& from_msg);
  public:
  PROTOBUF_ATTRIBUTE_REINITIALIZES void Clear() final;
  bool IsInitialized() const final;

  size_t ByteSizeLong() const final;
  const char* _InternalParse(const char* ptr, ::PROTOBUF_NAMESPACE_ID::internal::ParseContext* ctx) final;
  uint8_t* _InternalSerialize(
      uint8_t* target, ::PROTOBUF_NAMESPACE_ID::io::EpsCopyOutputStream* stream) const final;
  int GetCachedSize() const final { return _impl_._cached_size_.Get(); }

  private:
  void SharedCtor(::PROTOBUF_NAMESPACE_ID::Arena* arena, bool is_message_owned);
  void SharedDtor();
  void SetCachedSize(int size) const final;
  void InternalSwap(LoginRes* other);

  private:
  friend class ::PROTOBUF_NAMESPACE_ID::internal::AnyMetadata;
  static ::PROTOBUF_NAMESPACE_ID::StringPiece FullMessageName() {
    return "LoginRes";
  }
  protected:
  explicit LoginRes(::PROTOBUF_NAMESPACE_ID::Arena* arena,
                       bool is_message_owned = false);
  public:

  static const ClassData _class_data_;
  const ::PROTOBUF_NAMESPACE_ID::Message::ClassData*GetClassData() const final;

  ::PROTOBUF_NAMESPACE_ID::Metadata GetMetadata() const final;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  enum : int {
    kStatusFieldNumber = 1,
  };
  // int32 status = 1;
  void clear_status();
  int32_t status() const;
  void set_status(int32_t value);
  private:
  int32_t _internal_status() const;
  void _internal_set_status(int32_t value);
  public:

  // @@protoc_insertion_point(class_scope:LoginRes)
 private:
  class _Internal;

  template <typename T> friend class ::PROTOBUF_NAMESPACE_ID::Arena::InternalHelper;
  typedef void InternalArenaConstructable_;
  typedef void DestructorSkippable_;
  struct Impl_ {
    int32_t status_;
    mutable ::PROTOBUF_NAMESPACE_ID::internal::CachedSize _cached_size_;
  };
  union { Impl_ _impl_; };
  friend struct ::TableStruct_game_2eproto;
};
// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
// LoginReq

// string username = 1;
inline void LoginReq::clear_username() {
  _impl_.username_.ClearToEmpty();
}
inline const std::string& LoginReq::username() const {
  // @@protoc_insertion_point(field_get:LoginReq.username)
  return _internal_username();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LoginReq::set_username(ArgT0&& arg0, ArgT... args) {
 
 _impl_.username_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:LoginReq.username)
}
inline std::string* LoginReq::mutable_username() {
  std::string* _s = _internal_mutable_username();
  // @@protoc_insertion_point(field_mutable:LoginReq.username)
  return _s;
}
inline const std::string& LoginReq::_internal_username() const {
  return _impl_.username_.Get();
}
inline void LoginReq::_internal_set_username(const std::string& value) {
  
  _impl_.username_.Set(value, GetArenaForAllocation());
}
inline std::string* LoginReq::_internal_mutable_username() {
  
  return _impl_.username_.Mutable(GetArenaForAllocation());
}
inline std::string* LoginReq::release_username() {
  // @@protoc_insertion_point(field_release:LoginReq.username)
  return _impl_.username_.Release();
}
inline void LoginReq::set_allocated_username(std::string* username) {
  if (username != nullptr) {
    
  } else {
    
  }
  _impl_.username_.SetAllocated(username, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.username_.IsDefault()) {
    _impl_.username_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:LoginReq.username)
}

// int32 age = 2;
inline void LoginReq::clear_age() {
  _impl_.age_ = 0;
}
inline int32_t LoginReq::_internal_age() const {
  return _impl_.age_;
}
inline int32_t LoginReq::age() const {
  // @@protoc_insertion_point(field_get:LoginReq.age)
  return _internal_age();
}
inline void LoginReq::_internal_set_age(int32_t value) {
  
  _impl_.age_ = value;
}
inline void LoginReq::set_age(int32_t value) {
  _internal_set_age(value);
  // @@protoc_insertion_point(field_set:LoginReq.age)
}

// string email = 3;
inline void LoginReq::clear_email() {
  _impl_.email_.ClearToEmpty();
}
inline const std::string& LoginReq::email() const {
  // @@protoc_insertion_point(field_get:LoginReq.email)
  return _internal_email();
}
template <typename ArgT0, typename... ArgT>
inline PROTOBUF_ALWAYS_INLINE
void LoginReq::set_email(ArgT0&& arg0, ArgT... args) {
 
 _impl_.email_.Set(static_cast<ArgT0 &&>(arg0), args..., GetArenaForAllocation());
  // @@protoc_insertion_point(field_set:LoginReq.email)
}
inline std::string* LoginReq::mutable_email() {
  std::string* _s = _internal_mutable_email();
  // @@protoc_insertion_point(field_mutable:LoginReq.email)
  return _s;
}
inline const std::string& LoginReq::_internal_email() const {
  return _impl_.email_.Get();
}
inline void LoginReq::_internal_set_email(const std::string& value) {
  
  _impl_.email_.Set(value, GetArenaForAllocation());
}
inline std::string* LoginReq::_internal_mutable_email() {
  
  return _impl_.email_.Mutable(GetArenaForAllocation());
}
inline std::string* LoginReq::release_email() {
  // @@protoc_insertion_point(field_release:LoginReq.email)
  return _impl_.email_.Release();
}
inline void LoginReq::set_allocated_email(std::string* email) {
  if (email != nullptr) {
    
  } else {
    
  }
  _impl_.email_.SetAllocated(email, GetArenaForAllocation());
#ifdef PROTOBUF_FORCE_COPY_DEFAULT_STRING
  if (_impl_.email_.IsDefault()) {
    _impl_.email_.Set("", GetArenaForAllocation());
  }
#endif // PROTOBUF_FORCE_COPY_DEFAULT_STRING
  // @@protoc_insertion_point(field_set_allocated:LoginReq.email)
}

// int32 int_set = 4;
inline void LoginReq::clear_int_set() {
  _impl_.int_set_ = 0;
}
inline int32_t LoginReq::_internal_int_set() const {
  return _impl_.int_set_;
}
inline int32_t LoginReq::int_set() const {
  // @@protoc_insertion_point(field_get:LoginReq.int_set)
  return _internal_int_set();
}
inline void LoginReq::_internal_set_int_set(int32_t value) {
  
  _impl_.int_set_ = value;
}
inline void LoginReq::set_int_set(int32_t value) {
  _internal_set_int_set(value);
  // @@protoc_insertion_point(field_set:LoginReq.int_set)
}

// -------------------------------------------------------------------

// LoginRes

// int32 status = 1;
inline void LoginRes::clear_status() {
  _impl_.status_ = 0;
}
inline int32_t LoginRes::_internal_status() const {
  return _impl_.status_;
}
inline int32_t LoginRes::status() const {
  // @@protoc_insertion_point(field_get:LoginRes.status)
  return _internal_status();
}
inline void LoginRes::_internal_set_status(int32_t value) {
  
  _impl_.status_ = value;
}
inline void LoginRes::set_status(int32_t value) {
  _internal_set_status(value);
  // @@protoc_insertion_point(field_set:LoginRes.status)
}

#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__
// -------------------------------------------------------------------


// @@protoc_insertion_point(namespace_scope)


PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::Cmd> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Cmd>() {
  return ::Cmd_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_game_2eproto
