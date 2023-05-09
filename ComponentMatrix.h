#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace SUX
{
#if __cplusplus >= 202002L
	template <class T> 
	concept DerivedFromComponent = std::is_base_of<juce::Component, T>::value;
	template <class T> 
	concept DefaultConstructible = std::is_default_constructible<T>::value;

	template<typename ComponentType> requires DerivedFromComponent<ComponentType>&& DefaultConstructible<ComponentType>
#else
    template<typename ComponentType>
#endif
	class ComponentMatrix
	{
	public: 
		ComponentMatrix(const int rows, const int cols) : m_rows(rows), m_cols(cols) {
			m_cells.resize(static_cast<size_t>(rows) * static_cast<size_t>(cols));
		}

		ComponentType& at(int row, int col) {
			return m_cells[static_cast<size_t>((m_cols * row) + col)];
		}

		ComponentType& at(int index) {
			return m_cells[index];
		}

		void addAndMakeVisible(juce::Component* target) {
			for (auto& el : m_cells) {
				target->addAndMakeVisible(dynamic_cast<juce::Component*>(&el));
			}
		}

		ComponentType& operator[](size_t index) { return m_cells[index]; }

		typename std::vector<ComponentType>::iterator begin() { return m_cells.begin(); }
		typename std::vector<ComponentType>::iterator end() { return m_cells.end(); }

		int size() { return m_rows * m_cols; }
		[[nodiscard]] int rows() const { return m_rows; }
		[[nodiscard]] int cols() const { return m_cols; }

		std::tuple<int, int> getPositionForComponent(ComponentType* toFind)
		{
			auto it = std::find_if(m_cells.begin(), m_cells.end(), [toFind](const ComponentType& current) { return &current == toFind; });
			if (it == m_cells.end()) return { -1, -1 };
			int index = static_cast<int>(std::distance(m_cells.begin(), it));
			return unflattenIndex(index);
		}

		std::tuple<int, int> unflattenIndex(int flat) {
			// This assumes a square mat..
			int row = flat / m_cols;
			int col = flat % m_cols;
			return { row, col };
		}


	private: 
		std::vector<ComponentType> m_cells;
		const int m_rows, m_cols;
	};
}
