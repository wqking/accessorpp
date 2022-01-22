logicOperatorList = [
	'==', '!=', '>', '>=', '<', '<=',
	'&&', '||',
]
logicOperatorTemplate = '''
template <typename T, typename U>
auto operator {op} (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, bool>::type
{
	return (typename AccessorValueType<T>::Type)(a) {op} (typename AccessorValueType<U>::Type)(b);
}
'''

binaryOperatorList = [
	'+', '-', '*', '/', '%',
	'&', '|', '^', '<<', '>>',
]

binaryOperatorTemplate = '''
template <typename T, typename U>
auto operator {op} (const T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T>::type
{
	T result(a);
	result = (typename AccessorValueType<T>::Type)(a) {op} (typename AccessorValueType<U>::Type)(b);
	return result;
}
'''

binaryAssignOperatorList = [
	'+=', '-=', '*=', '/=', '%=',
	'&=', '|=', '^=', '<<=', '>>=',
]

binaryAssignOperatorTemplate = '''
template <typename T, typename U>
auto operator {op} (T & a, const U & b)
	-> typename std::enable_if<IsAccessor<T>::value, T &>::type
{
	a = (typename AccessorValueType<T>::Type)(a) {rop} (typename AccessorValueType<U>::Type)(b);
	return a;
}
'''

def doGenerate(operatorList, operatorTemplate) :
	for operator in operatorList :
		rop = operator.replace('=', '')
		code = operatorTemplate;
		code = code.replace('{op}', operator)
		code = code.replace('{rop}', rop)
		print(code, end = '')

print('// Logic operators')
doGenerate(logicOperatorList, logicOperatorTemplate)
print('// Binary operators')
doGenerate(binaryOperatorList, binaryOperatorTemplate)
print('// Binary assignment operators')
doGenerate(binaryAssignOperatorList, binaryAssignOperatorTemplate)
