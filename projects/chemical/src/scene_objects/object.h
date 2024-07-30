#pragma once

#include <vector>
#include <memory>

namespace Chemical {

	namespace Scene {

		/*template<typename Type>
		class Superior {

			using Descriptor = std::string;

			const std::string& GetDescriptor() {
				return descriptor;
			}
			Type& Get() {
				return superior_instance;
			}
			Superior(Type& superior_instance, const Descriptor& descriptor) :
				superior_instance(superior_instance), descriptor(descriptor) {}

		private:
			Type& superior_instance;
			SuperiorTypeName descriptor;
		};*/
		
		class Object {
		public:
			using SuperiorDescriptor = std::string;

			Object(void* superior_instance, const SuperiorDescriptor& superior_descriptor) :
				m_superior(superior_instance), m_superior_descriptor(superior_descriptor) {}

			const SuperiorDescriptor& GetSuperiorDescriptor() const {
				return m_superior_descriptor;
			}
			template<typename T>
			T& GetSuperior() const {
				return *static_cast<T*>(m_superior);
			}

			// copies child
			void AddChild(const Object& child) {
				m_children.emplace_back(child);
			}
			std::vector<Object>& GetChildren() {
				return m_children;
			}

		private:
			std::vector<Object> m_children;

			void* m_superior;
			SuperiorDescriptor m_superior_descriptor;
		};
	}

}
