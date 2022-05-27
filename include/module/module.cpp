#include "module/module.h"

using namespace std;

Module::Module(){
	
}

Module::~Module() {
	
}

// ������Կ���һ���Ƿ���Ҫ���첽����Ϊ���������ɲ���ʹ�ã���UI�У���Ҫ����һ���߳���ʹ�ã�����Ῠס���̡߳�
// �Ƿ���Ҫ�ص�����ΰ�װ�����Ͳ����ء�
void Module::Init(Ort::Env& env, Ort::SessionOptions& session_op, const wchar_t* model_path,Ort::AllocatorWithDefaultOptions& allocator) {
	session = make_shared<Ort::Session>(env, model_path, session_op);
	model_info = std::move(onnxutils::ModelIOInfo(&(*session), &allocator));
}