#ifndef _IV_LV5_RUNTIME_ARRAY_H_
#define _IV_LV5_RUNTIME_ARRAY_H_
#include "arguments.h"
#include "jsval.h"
#include "error.h"
#include "jsarray.h"
#include "jsstring.h"
#include "conversions.h"
#include "internal.h"
#include "runtime_object.h"

namespace iv {
namespace lv5 {
namespace runtime {

// section 15.4.1.1 Array([item0 [, item1 [, ...]]])
// section 15.4.2.1 new Array([item0 [, item1 [, ...]]])
// section 15.4.2.2 new Array(len)
inline JSVal ArrayConstructor(const Arguments& args, Error* error) {
// TODO(Constellation) this is mock function
  return JSArray::New(args.ctx());
}

// section 15.4.4.2 Array.prototype.toString()
inline JSVal ArrayToString(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.toString", args, error);
  JSObject* const obj = args.this_binding().ToObject(args.ctx(), ERROR(error));
  const JSVal join = obj->Get(args.ctx(),
                              args.ctx()->Intern("join"), ERROR(error));
  if (join.IsCallable()) {
    return join.object()->AsCallable()->Call(
        Arguments(args.ctx(), obj), ERROR(error));
  } else {
    return ObjectToString(Arguments(args.ctx(), obj), ERROR(error));
  }
}

// section 15.4.4.4 Array.prototype.concat([item1[, item2[, ...]]])
inline JSVal ArrayConcat(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.concat", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  JSArray* const ary = JSArray::New(ctx);

  uint32_t n = 0;
  const Class& cls = ctx->Cls("Array");
  std::tr1::array<char, 20> buf;

  if (cls.name == obj->cls()) {
    JSObject* const elm = obj;
    uint32_t k = 0;
    const JSVal length = elm->Get(
        ctx,
        ctx->length_symbol(), ERROR(error));
    assert(length.IsNumber());  // Array always number
    const uint32_t len = core::DoubleToUInt32(length.number());
    while (k < len) {
      const Symbol index = ctx->Intern(
          core::StringPiece(
              buf.data(),
              std::snprintf(buf.data(), buf.size(), "%lu",
                            static_cast<unsigned long>(k))));  // NOLINT
      if (elm->HasProperty(index)) {
        const JSVal subelm = elm->Get(ctx, index, ERROR(error));
        ary->DefineOwnProperty(
            ctx,
            ctx->Intern(
                core::StringPiece(
                    buf.data(),
                    std::snprintf(
                        buf.data(), buf.size(), "%lu",
                        static_cast<unsigned long>(n)))),  // NOLINT
            DataDescriptor(subelm,
                           PropertyDescriptor::WRITABLE |
                           PropertyDescriptor::ENUMERABLE |
                           PropertyDescriptor::CONFIGURABLE),
            false, ERROR(error));
      }
      ++n;
      ++k;
    }
  } else {
    ary->DefineOwnProperty(
        ctx,
        ctx->Intern(
            core::StringPiece(
                buf.data(),
                std::snprintf(
                    buf.data(), buf.size(), "%lu",
                    static_cast<unsigned long>(n)))),  // NOLINT
        DataDescriptor(obj,
                       PropertyDescriptor::WRITABLE |
                       PropertyDescriptor::ENUMERABLE |
                       PropertyDescriptor::CONFIGURABLE),
        false, ERROR(error));
    ++n;
  }

  for (Arguments::const_iterator it = args.begin(),
       last = args.end(); it != last; ++it) {
    if (it->IsObject() && cls.name == it->object()->cls()) {
      JSObject* const elm = it->object();
      uint32_t k = 0;
      const JSVal length = elm->Get(
          ctx,
          ctx->length_symbol(), ERROR(error));
      assert(length.IsNumber());  // Array always number
      const uint32_t len = core::DoubleToUInt32(length.number());
      while (k < len) {
        const Symbol index = ctx->Intern(
            core::StringPiece(
                buf.data(),
                std::snprintf(buf.data(), buf.size(), "%lu",
                              static_cast<unsigned long>(k))));  // NOLINT
        if (elm->HasProperty(index)) {
          const JSVal subelm = elm->Get(ctx, index, ERROR(error));
          ary->DefineOwnProperty(
              ctx,
              ctx->Intern(
                  core::StringPiece(
                      buf.data(),
                      std::snprintf(
                          buf.data(), buf.size(), "%lu",
                          static_cast<unsigned long>(n)))),  // NOLINT
              DataDescriptor(subelm,
                             PropertyDescriptor::WRITABLE |
                             PropertyDescriptor::ENUMERABLE |
                             PropertyDescriptor::CONFIGURABLE),
              false, ERROR(error));
        }
        ++n;
        ++k;
      }
    } else {
      ary->DefineOwnProperty(
          ctx,
          ctx->Intern(
              core::StringPiece(
                  buf.data(),
                  std::snprintf(
                      buf.data(), buf.size(), "%lu",
                      static_cast<unsigned long>(n)))),  // NOLINT
          DataDescriptor(*it,
                         PropertyDescriptor::WRITABLE |
                         PropertyDescriptor::ENUMERABLE |
                         PropertyDescriptor::CONFIGURABLE),
          false, ERROR(error));
      ++n;
    }
  }
  return ary;
}

// section 15.4.4.5 Array.prototype.join(separator)
inline JSVal ArrayJoin(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.join", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t len = core::DoubleToUInt32(val);
  JSString* separator;
  if (args.size() > 0 && !args[0].IsUndefined()) {
    separator = args[0].ToString(ctx, ERROR(error));
  } else {
    separator = JSString::NewAsciiString(ctx, ",");
  }
  if (len == 0) {
    return JSString::NewEmptyString(ctx);
  }
  JSStringBuilder builder(ctx);
  {
    const JSVal element0 = obj->Get(ctx, ctx->Intern("0"), ERROR(error));
    if (!element0.IsUndefined() && !element0.IsNull()) {
      const JSString* const str = element0.ToString(ctx, ERROR(error));
      builder.Append(*str);
    }
  }
  uint32_t k = 1;
  std::tr1::array<char, 20> buf;
  while (k < len) {
    builder.Append(*separator);
    const int num = std::snprintf(buf.data(), buf.size(), "%lu",
                                  static_cast<unsigned long>(k));  // NOLINT
    const JSVal element = obj->Get(
        ctx,
        ctx->Intern(core::StringPiece(buf.data(), num)),
        ERROR(error));
    if (!element.IsUndefined() && !element.IsNull()) {
      const JSString* const str = element.ToString(ctx, ERROR(error));
      builder.Append(*str);
    }
    k += 1;
  }
  return builder.Build();
}

// section 15.4.4.6 Array.prototype.pop()
inline JSVal ArrayPop(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.pop", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t len = core::DoubleToUInt32(val);
  if (len == 0) {
    obj->Put(ctx, ctx->length_symbol(), 0.0, true, ERROR(error));
    return JSUndefined;
  } else {
    std::tr1::array<char, 20> buf;
    const uint32_t index = len - 1;
    const Symbol indx = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(index))));  // NOLINT
    const JSVal element = obj->Get(ctx, indx, ERROR(error));
    obj->Delete(indx, true, ERROR(error));
    obj->Put(ctx, ctx->length_symbol(),
             index, true, ERROR(error));
    return element;
  }
}

// section 15.4.4.7 Array.prototype.push([item1[, item2[, ...]]])
inline JSVal ArrayPush(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.push", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  uint32_t n = core::DoubleToUInt32(val);
  std::tr1::array<char, 20> buf;
  for (Arguments::const_iterator it = args.begin(),
       last = args.end(); it != last; ++it, ++n) {
    obj->Put(
        ctx,
        ctx->Intern(
            core::StringPiece(
                buf.data(),
                std::snprintf(
                    buf.data(), buf.size(), "%lu",
                    static_cast<unsigned long>(n)))),  // NOLINT
        *it,
        true, ERROR(error));
  }
  obj->Put(
      ctx,
      ctx->length_symbol(),
      n,
      true, ERROR(error));
  return n;
}

// section 15.4.4.8 Array.prototype.reverse()
inline JSVal ArrayReverse(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.reverse", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t len = core::DoubleToUInt32(val);
  const uint32_t middle = len >> 1;
  std::tr1::array<char, 20> buf;
  for (uint32_t lower = 0; lower != middle; ++lower) {
    const uint32_t upper = len - lower - 1;
    const Symbol lower_symbol = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(lower))));  // NOLINT
    const Symbol upper_symbol = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(upper))));  // NOLINT
    const JSVal lower_value = obj->Get(
        ctx,
        lower_symbol,
        ERROR(error));
    const JSVal upper_value = obj->Get(
        ctx,
        upper_symbol,
        ERROR(error));
    const bool lower_exists = obj->HasProperty(lower_symbol);
    const bool upper_exists = obj->HasProperty(upper_symbol);
    if (lower_exists && upper_exists) {
      obj->Put(ctx, lower_symbol, upper_value, true, ERROR(error));
      obj->Put(ctx, upper_symbol, lower_value, true, ERROR(error));
    } else if (!lower_exists && upper_exists) {
      obj->Put(ctx, lower_symbol, upper_value, true, ERROR(error));
      obj->Delete(upper_symbol, true, ERROR(error));
    } else if (lower_exists && !upper_exists) {
      obj->Delete(lower_symbol, true, ERROR(error));
      obj->Put(ctx, upper_symbol, lower_value, true, ERROR(error));
    } else {
      // no action is required
    }
  }
  return obj;
}

// section 15.4.4.9 Array.prototype.shift()
inline JSVal ArrayShift(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.shift", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t len = core::DoubleToUInt32(val);
  if (len == 0) {
    obj->Put(ctx, ctx->length_symbol(), 0.0, true, ERROR(error));
    return JSUndefined;
  }
  const JSVal first = obj->Get(ctx, ctx->Intern("0"), ERROR(error));
  std::tr1::array<char, 20> buf;
  Symbol to = ctx->Intern(
      core::StringPiece(
          buf.data(),
          std::snprintf(
              buf.data(), buf.size(), "%lu",
              static_cast<unsigned long>(0))));  // NOLINT
  Symbol from;
  for (uint32_t k = 1; k < len; ++k, to = from) {
    from = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(k))));  // NOLINT
    if (obj->HasProperty(from)) {
      const JSVal from_value = obj->Get(ctx, from, ERROR(error));
      obj->Put(ctx, to, from_value, true, ERROR(error));
    } else {
      obj->Delete(to, true, ERROR(error));
    }
  }
  obj->Delete(from, true, ERROR(error));
  obj->Put(ctx, ctx->length_symbol(), len - 1, true, ERROR(error));
  return first;
}

// section 15.4.4.10 Array.prototype.slice(start, end)
inline JSVal ArraySlice(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.slice", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  JSArray* const ary = JSArray::New(ctx);
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t len = core::DoubleToUInt32(val);
  uint32_t k;
  if (args.size() > 0) {
    double relative_start = args[0].ToNumber(ctx, ERROR(error));
    relative_start = core::DoubleToInteger(relative_start);
    if (relative_start < 0) {
      k = core::DoubleToUInt32(std::max<double>(relative_start + len, 0.0));
    } else {
      k = core::DoubleToUInt32(std::min<double>(relative_start, len));
    }
  } else {
    k = 0;
  }
  uint32_t final;
  if (args.size() > 1) {
    if (args[1].IsUndefined()) {
      final = len;
    } else {
      double relative_end = args[1].ToNumber(ctx, ERROR(error));
      relative_end = core::DoubleToInteger(relative_end);
      if (relative_end < 0) {
        final = core::DoubleToUInt32(std::max<double>(relative_end + len, 0.0));
      } else {
        final = core::DoubleToUInt32(std::min<double>(relative_end , len));
      }
    }
  } else {
    final = len;
  }
  std::tr1::array<char, 20> buf;
  for (uint32_t n = 0; k < final; ++k, ++n) {
    const Symbol pk = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(k))));  // NOLINT
    if (obj->HasProperty(pk)) {
      const JSVal kval = obj->Get(ctx, pk, ERROR(error));
      ary->DefineOwnProperty(
          ctx,
          ctx->Intern(
              core::StringPiece(
                  buf.data(),
                  std::snprintf(
                      buf.data(), buf.size(), "%lu",
                      static_cast<unsigned long>(n)))),  // NOLINT
          DataDescriptor(kval,
                         PropertyDescriptor::WRITABLE |
                         PropertyDescriptor::ENUMERABLE |
                         PropertyDescriptor::CONFIGURABLE),
          false, ERROR(error));
    }
  }
  return ary;
}

// section 15.4.4.12 Array.prototype.splice(start, deleteCount[, item1[, item2[, ...]]])  // NOLINT
inline JSVal ArraySplice(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.splice", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  JSArray* const ary = JSArray::New(ctx);
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t len = core::DoubleToUInt32(val);
  const uint32_t args_size = args.size();
  uint32_t actual_start;
  if (args_size > 0) {
    double relative_start = args[0].ToNumber(ctx, ERROR(error));
    relative_start = core::DoubleToInteger(relative_start);
    if (relative_start < 0) {
      actual_start = core::DoubleToUInt32(
          std::max<double>(relative_start + len, 0.0));
    } else {
      actual_start = core::DoubleToUInt32(
          std::min<double>(relative_start, len));
    }
  } else {
    actual_start = 0;
  }
  uint32_t actual_delete_count;
  if (args_size > 1) {
    double delete_count = args[1].ToNumber(ctx, ERROR(error));
    delete_count = core::DoubleToInteger(delete_count);
    actual_delete_count = core::DoubleToUInt32(
        std::min<double>(
            std::max<double>(delete_count, 0.0), len - actual_start));
  } else {
    actual_delete_count = std::min<uint32_t>(0, len - actual_start);
  }
  std::tr1::array<char, 20> buf;
  for (uint32_t k = 0; k < actual_delete_count; ++k) {
    const Symbol from = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(actual_start + k))));  // NOLINT
    if (obj->HasProperty(from)) {
      const JSVal from_val = obj->Get(ctx, from, ERROR(error));
      ary->DefineOwnProperty(
          ctx,
          ctx->Intern(
              core::StringPiece(
                  buf.data(),
                  std::snprintf(
                      buf.data(), buf.size(), "%lu",
                      static_cast<unsigned long>(k)))),  // NOLINT
          DataDescriptor(from_val,
                         PropertyDescriptor::WRITABLE |
                         PropertyDescriptor::ENUMERABLE |
                         PropertyDescriptor::CONFIGURABLE),
          false, ERROR(error));
    }
  }

  const uint32_t item_count = (args_size > 2)? args_size - 2 : 0;
  if (item_count < actual_delete_count) {
    for (uint32_t k = actual_start,
         last = len - actual_delete_count; k < last; ++k) {
      const Symbol from = ctx->Intern(
          core::StringPiece(
              buf.data(),
              std::snprintf(
                  buf.data(), buf.size(), "%lu",
                  static_cast<unsigned long>(
                      k + actual_delete_count))));  // NOLINT
      const Symbol to = ctx->Intern(
          core::StringPiece(
              buf.data(),
              std::snprintf(
                  buf.data(), buf.size(), "%lu",
                  static_cast<unsigned long>(
                      k + item_count))));  // NOLINT
      if (obj->HasProperty(from)) {
        const JSVal from_value = obj->Get(ctx, from, ERROR(error));
        obj->Put(ctx, to, from_value, true, ERROR(error));
      } else {
        obj->Delete(to, true, ERROR(error));
      }
    }
  } else if (item_count > actual_delete_count) {
    for (uint32_t k = len - actual_delete_count; actual_start < k; --k) {
      const Symbol from = ctx->Intern(
          core::StringPiece(
              buf.data(),
              std::snprintf(
                  buf.data(), buf.size(), "%lu",
                  static_cast<unsigned long>(
                      k + actual_delete_count - 1))));  // NOLINT
      const Symbol to = ctx->Intern(
          core::StringPiece(
              buf.data(),
              std::snprintf(
                  buf.data(), buf.size(), "%lu",
                  static_cast<unsigned long>(
                      k + item_count - 1))));  // NOLINT
      if (obj->HasProperty(from)) {
        const JSVal from_value = obj->Get(ctx, from, ERROR(error));
        obj->Put(ctx, to, from_value, true, ERROR(error));
      } else {
        obj->Delete(to, true, ERROR(error));
      }
    }
  }
  Arguments::const_iterator it = args.begin();
  std::advance(it, 2);
  for (uint32_t k = 0; k < item_count ; ++k, ++it) {
    obj->Put(
        ctx,
        ctx->Intern(
            core::StringPiece(
                buf.data(),
                std::snprintf(
                    buf.data(), buf.size(), "%lu",
                    static_cast<unsigned long>(
                        k + item_count - 1)))),  // NOLINT
        *it, true, ERROR(error));
  }
  obj->Put(
      ctx,
      ctx->length_symbol(),
      len - actual_delete_count + item_count, true, ERROR(error));
  return ary;
}

// section 15.4.4.13 Array.prototype.unshift([item1[, item2[, ...]]])
inline JSVal ArrayUnshift(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.unshift", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t arg_count = args.size();
  const uint32_t len = core::DoubleToUInt32(val);

  std::tr1::array<char, 20> buf;

  for (uint32_t k = len; k > 0; --k) {
    const Symbol from = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(k - 1))));  // NOLINT
    const Symbol to = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(k + arg_count - 1))));  // NOLINT
    if (obj->HasProperty(from)) {
      const JSVal from_value = obj->Get(ctx, from, ERROR(error));
      obj->Put(ctx, to, from_value, true, ERROR(error));
    } else {
      obj->Delete(to, true, ERROR(error));
    }
  }

  uint32_t j = 0;
  for (Arguments::const_iterator it = args.begin(),
       last = args.end(); it != last; ++it, ++j) {
    obj->Put(
        ctx,
        ctx->Intern(
            core::StringPiece(
                buf.data(),
                std::snprintf(
                    buf.data(), buf.size(), "%lu",
                    static_cast<unsigned long>(j)))),  // NOLINT
        *it,
        true, ERROR(error));
  }
  obj->Put(
      ctx,
      ctx->length_symbol(),
      len + arg_count,
      true, ERROR(error));
  return len + arg_count;
}

// section 15.4.4.14 Array.prototype.indexOf(searchElement[, fromIndex])
inline JSVal ArrayIndexOf(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.indexOf", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t arg_count = args.size();
  const uint32_t len = core::DoubleToUInt32(val);

  if (len == 0) {
    return -1;
  }

  JSVal search_element;
  if (arg_count > 0) {
    search_element = args[0];
  } else {
    search_element = JSUndefined;
  }

  uint32_t k;
  if (arg_count > 1) {
    double fromIndex = args[1].ToNumber(ctx, ERROR(error));
    fromIndex = core::DoubleToInteger(fromIndex);
    if (fromIndex >= len) {
      return -1;
    }
    if (fromIndex >= 0) {
      k = core::DoubleToUInt32(fromIndex);
    } else {
      if (len < -fromIndex) {
        k = 0;
      } else {
        k = len - core::DoubleToUInt32(-fromIndex);
      }
    }
  } else {
    k = 0;
  }

  std::tr1::array<char, 20> buf;
  for (; k < len; ++k) {
    const Symbol sym = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(k))));  // NOLINT
    if (obj->HasProperty(sym)) {
      const JSVal element_k = obj->Get(ctx, sym, ERROR(error));
      if (StrictEqual(search_element, element_k)) {
        return k;
      }
    }
  }
  return -1;
}

// section 15.4.4.15 Array.prototype.lastIndexOf(searchElement[, fromIndex])
inline JSVal ArrayLastIndexOf(const Arguments& args, Error* error) {
  CONSTRUCTOR_CHECK("Array.prototype.lastIndexOf", args, error);
  Context* const ctx = args.ctx();
  JSObject* const obj = args.this_binding().ToObject(ctx, ERROR(error));
  const JSVal length = obj->Get(
      ctx,
      ctx->length_symbol(), ERROR(error));
  const double val = length.ToNumber(ctx, ERROR(error));
  const uint32_t arg_count = args.size();
  const uint32_t len = core::DoubleToUInt32(val);

  if (len == 0) {
    return -1;
  }

  JSVal search_element;
  if (arg_count > 0) {
    search_element = args[0];
  } else {
    search_element = JSUndefined;
  }

  uint32_t k;
  if (arg_count > 1) {
    double fromIndex = args[1].ToNumber(ctx, ERROR(error));
    fromIndex = core::DoubleToInteger(fromIndex);
    if (fromIndex >= 0) {
      if (fromIndex > (len - 1)) {
        k = len - 1;
      } else {
        k = core::DoubleToUInt32(fromIndex);
      }
    } else {
      if (-fromIndex > len) {
        return -1;
      } else {
        k = len - core::DoubleToUInt32(-fromIndex);
      }
    }
  } else {
    k = len;
  }

  std::tr1::array<char, 20> buf;
  while (k--) {
    const Symbol sym = ctx->Intern(
        core::StringPiece(
            buf.data(),
            std::snprintf(
                buf.data(), buf.size(), "%lu",
                static_cast<unsigned long>(k))));  // NOLINT
    if (obj->HasProperty(sym)) {
      const JSVal element_k = obj->Get(ctx, sym, ERROR(error));
      if (StrictEqual(search_element, element_k)) {
        return k;
      }
    }
  }
  return -1;
}

} } }  // namespace iv::lv5::runtime
#endif  // _IV_LV5_RUNTIME_ARRAY_H_
