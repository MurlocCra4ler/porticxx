[[noreturn]] void unreachable_rtti_call() { __builtin_trap(); }

namespace __cxxabiv1 {

struct __class_type_info {
    virtual ~__class_type_info() = default;

    virtual void __do_find_public_src() const { unreachable_rtti_call(); }

    virtual void __do_dyncast() const { unreachable_rtti_call(); }

    virtual void __do_upcast() const { unreachable_rtti_call(); }
};

struct __si_class_type_info : __class_type_info {
    __class_type_info* __base_type;

    virtual ~__si_class_type_info() = default;

    virtual void __do_find_public_src() const { unreachable_rtti_call(); }

    virtual void __do_dyncast() const { unreachable_rtti_call(); }

    virtual void __do_upcast() const { unreachable_rtti_call(); }
};

struct __vmi_class_type_info : __class_type_info {
    __class_type_info* __base_type;

    virtual ~__vmi_class_type_info() = default;

    virtual void __do_find_public_src() const { unreachable_rtti_call(); }

    virtual void __do_dyncast() const { unreachable_rtti_call(); }

    virtual void __do_upcast() const { unreachable_rtti_call(); }
};

__class_type_info dummy_info;
__si_class_type_info si_dummy_info;
__vmi_class_type_info vmi_si_dummy_info;

} // namespace __cxxabiv1
