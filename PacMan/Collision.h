#pragma once

template <typename A>
concept rect = requires (A a) { a.x; a.y; a.w; a.h; };

template <rect A,rect B>
inline bool kolizja(A&& a, B&& b) noexcept
{
	return a.x < (b.x + b.w) && (a.x + a.w) > b.x && a.y < (b.y + b.h) && (a.y + a.h) > b.y;
}

template <rect A,rect B>
inline int kolizja(const A& a, const B& b) noexcept
{
	if (a.x < (b.x + b.w) && (a.x + a.w) > b.x && a.y < (b.y + b.h) && (a.y + a.h) > b.y)
	{
		return 1;
	}
	return 0;
}

template <rect A, rect B>
inline A& operator+(A& a, B&& b) noexcept
{
	a.x = std::max(a.x, b.x - a.w);
	a.y = std::max(a.y, b.y - a.h);
	a.w = b.x > 0 ? (b.x + b.w - a.x) : a.w;
	a.h = b.y > 0 ? (b.y + b.h - a.y) : a.h;
	return a;
}

template <rect A, typename B>
inline A& operator*(A& a, B& i) noexcept
{
	a.x *= i;
	a.y *= i;
	a.h *= i;
	a.w *= i;
	return a;
}

template <rect A>
inline auto gKolizjiX(const A& a, const A& b)
{
	auto xDeph = std::min(a.x + a.w, b.x + b.w) - std::max(a.x, b.x);
	return std::move(xDeph);
}

template <rect A, rect B>
inline auto gKolizjiX(const A& a, const B& b)
{
	auto xDeph = std::min(static_cast<float>(a.x) + static_cast<float>(a.w), static_cast<float>(b.x) + static_cast<float>(b.w)) - std::max(static_cast<float>(a.x), static_cast<float>(b.x));
	return std::move(xDeph);
}

template <rect A>
inline auto gKolizjiY(const A& a, const A& b)
{
	auto yDeph = std::min(a.y + a.h, b.y + b.h) - std::max(a.y, b.y);
	return std::move(yDeph);
}

template <rect A, rect B>
inline auto gKolizjiY(const A& a, const B& b)
{
	auto yDeph = std::min(static_cast<float>(a.y) + static_cast<float>(a.h), static_cast<float>(b.y) + static_cast<float>(b.h)) - std::max(static_cast<float>(a.y), static_cast<float>(b.y));
	return std::move(yDeph);
}

template <rect A>
inline auto gKolizjiXY(const A& a, const A& b)
{
	return gKolizjiY(a, b) * gKolizjiX(a, b);
}

template <rect A, rect B>
inline auto gKolizjiXY(const A& a, const B& b)
{
	return gKolizjiY(a, b) * gKolizjiX(a, b);
}

template <rect A>
inline auto AreSame(A& x, A& y) noexcept
{
	return (x.x == y.x) && (x.y == y.y);
}