#pragma once
#include <stdexcept>

#pragma push_macro("_SCOPE")
#define _SCOPE protected

#define EXCEPTION_CLASS(_DerivedException, _BaseException)\
		class _DerivedException : public _BaseException {\
			_SCOPE:\
				_DerivedException(const std::string& message) :\
					_BaseException(message.c_str())\
				{}\
		}\

EXCEPTION_CLASS(CollectionException, std::exception);

#undef _SCOPE
#define _SCOPE public

EXCEPTION_CLASS(EmptyCollectionException, CollectionException);

#undef _SCOPE
#pragma pop_macro("_SCOPE")