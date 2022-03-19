#ifndef		TEMP_ARRAY_HPP
#define		TEMP_ARRAY_HPP

#include <cstddef>
#include <cassert>
#include <utility>
#include <iterator>
#include <type_traits>

namespace lib_fm {
	using std::move;
	using std::size_t;
	using std::forward;
	using std::exchange;
	using std::is_same_v;
	using std::declval;
	using std::make_signed_t;
	using std::initializer_list;
	using std::is_convertible_v;
	using std::is_constructible_v;
	using std::remove_reference_t;

	using signed_size_t = make_signed_t<size_t>;

	template<typename vtype>
		requires(is_same_v<remove_reference_t<vtype>, vtype>)
	struct initializer_array;

	namespace details {
		struct kill_temporary_array {
			~kill_temporary_array() noexcept {
				if (target)
					*target = 0;
			};
		private:
			template<typename vtype>
				requires(is_same_v<remove_reference_t<vtype>, vtype>)
			friend struct lib_fm::initializer_array;

			constexpr bool operator=(signed_size_t& bomb) && noexcept {
				if (target)
					return false;
				target = &bomb;
				return true;
			};

			void operator=(kill_temporary_array&&) = delete;
			void operator&() = delete;
			signed_size_t* target = nullptr;
		};
	};// ! details

	template<typename vtype>
		requires(is_same_v<remove_reference_t<vtype>, vtype>)
	struct initializer_array {
		static_assert(is_same_v<remove_reference_t<vtype>, vtype>);
		using reference = vtype&&;

		template<signed_size_t init_count>
		constexpr initializer_array(vtype(&& ref)[init_count], details::kill_temporary_array&& bomb = {})
			noexcept
			: start{ ref + init_count }
			, count{ -init_count }
		{
			bool const chk{ forward<details::kill_temporary_array&&>(bomb) = count };
			(void)chk;
			assert(chk&&" second parameter must default");
		};

		constexpr decltype(auto) begin() noexcept
		{
			return iterator<> {this};
		};

		constexpr decltype(auto) rbegin() noexcept
		{
			return iterator<true> {this};
		};

		constexpr auto end() const noexcept
		{
			//return iterator<>{};
			return nullptr;
		};

		constexpr auto size() const noexcept
		{
			return -count;
		};

		constexpr auto rend() const noexcept
		{
			//return iterator<true>{};
			return nullptr;
		};

		//constexpr decltype(auto) front() const noexcept
		//{
		//	return move(start[count]);
		//};

		//constexpr decltype(auto) back() const noexcept
		//{
		//	return move(start[-1]);
		//};

		template<bool reverse = false>
		struct iterator
		{
			using iterator_category = std::input_iterator_tag;
			using difference_type = std::ptrdiff_t;
			typedef vtype value_type, &reference, * pointer;

			constexpr explicit iterator()noexcept = default;
			constexpr iterator(initializer_array* oth) noexcept
				: count{ exchange(oth->count,0) }
				, start{ oth->start + reverse * count }
			{
				if constexpr (reverse)
					count = -count;
			};

			constexpr reference operator*() const noexcept
			{
				return static_cast<reference>(start[count - reverse]);
			};

			constexpr pointer operator->() const noexcept
			{
				return start + count - reverse;
			};

			constexpr auto& operator++() noexcept
			{
				if constexpr (reverse)
					--count;
				else
					++count;
				return *this;
			};

			constexpr void operator++(int) noexcept
			{
				operator++();
			};

			constexpr explicit operator bool() const noexcept
			{
				return count;
			};

			constexpr bool operator!() const noexcept
			{
				return !count;
			};

			constexpr bool operator==(decltype(nullptr)const) const noexcept
			{
				return !count;
			};

			friend constexpr bool operator==(decltype(nullptr)const, iterator const& that) noexcept
			{
				return !that.count;
			};

			constexpr bool operator==(iterator const& rhs) const noexcept { 
				assert((!rhs) && !rhs.start);
				return !count;
			};

			constexpr bool operator!=(iterator const& rhs) const noexcept { return count; };
		private:
			signed_size_t count=0;
			vtype* const start=nullptr;
		};

		constexpr ~initializer_array() noexcept {
			count = 0;
		};

	private:
		constexpr initializer_array operator=(initializer_array&& oth) = delete;
		void operator&() = delete;

		vtype* const start;
		signed_size_t count;
	};

	template<typename container>
	struct replicate_initializer
		: container
	{
		using typename container::value_type;
		using typename container::allocator_type;
		using init_iter = typename initializer_array<value_type>::template iterator<false>;

		using container::container;

		explicit constexpr replicate_initializer(initializer_array<value_type> init ,allocator_type const& alloc)
		requires(is_constructible_v<container, initializer_list<value_type>, allocator_type const&>)
			: container
			{ std::make_move_iterator(move(init).begin()), std::make_move_iterator( decltype(init.begin()) {}), alloc}
		{};

		constexpr replicate_initializer(initializer_array<value_type> init)
		requires(is_constructible_v<container, initializer_list<value_type>>)
			: container
			{ std::make_move_iterator(move(init).begin()), std::make_move_iterator(decltype(init.begin()) {}) }
		{};
	private:
		replicate_initializer(initializer_list<value_type>, allocator_type const&)
			requires(std::is_constructible_v<container, initializer_list<value_type>, allocator_type const&>)
		= delete;

		replicate_initializer(initializer_list<value_type>)
			requires(std::is_constructible_v<container, initializer_list<value_type>>)
		= delete;
	};// ! replicate_initializer
};// ! lib_fm

#endif // !	TEMP_ARRAY_HPP
