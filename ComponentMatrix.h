#pragma once
#include <juce_gui_basics/juce_gui_basics.h>
namespace SUX
{
	template <class T> 
	concept DerivedFromComponent = std::is_base_of<juce::Component, T>::value;
	template <class T> 
	concept DefaultConstructible = std::is_default_constructible<T>::value;

	template<typename ComponentType, int Rows, int Cols> requires DerivedFromComponent<ComponentType> && DefaultConstructible<ComponentType>
	class ComponentMatrix
	{
	public: 
		ComponentType& at(int row, int col) {
			return m_cells[(Cols * row) + col];
		}

		void addAndMakeVisible(juce::Component* target) {
			for (auto& el : m_cells) {
				target->addAndMakeVisible(dynamic_cast<juce::Component*>(&el));
			}
		}
	
		ComponentType& operator[](size_t index) { return m_cells[index]; }
        
        typename std::array<ComponentType, Rows* Cols>::iterator begin() { return m_cells.begin(); }
        
        typename std::array<ComponentType, Rows* Cols>::iterator end() { return m_cells.end(); }

		constexpr int size() { return Rows * Cols; }
		constexpr int rows() { return Rows; }
		constexpr int cols() { return Cols; }

		std::tuple<int, int> getPositionForComponent(ComponentType* toFind)
		{
			auto it = std::find_if(m_cells.begin(), m_cells.end(), [toFind](const ComponentType& current) { return &current == toFind; });
			if (it == m_cells.end()) return { -1, -1 };
			int index = std::distance(m_cells.begin(), it);
			return unflattenIndex(index);
		}

		std::tuple<int, int> unflattenIndex(int flat) {
			int row = flat / Rows;
			int col = flat % Rows;
			return { row, col };
		}
	private:
		std::array<ComponentType, Rows * Cols> m_cells;
	};
}
