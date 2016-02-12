/*
 * Copyright 2016 Collapsed Team
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#pragma once

template <class Type>
class PoinT
{
public:
	PoinT()
		: x(static_cast<Type>(x)), y(static_cast<Type>(y))
	{ }

	PoinT(Type x, Type y)
		: x(x), y(y)
	{ }

	inline double distance(Type x, Type y) { 
		return sqrt(static_cast<double>((this->x - x) * (this->x - x)) + 
					static_cast<double>((this->y - y) * (this->y - y))); 
	}

	inline double distance(const PoinT& point) { 
		return distance(point.x, point.y); 
	}

	inline PoinT<s32> toPoint() {
		return PoinT<s32>(static_cast<s32>(x), static_cast<s32>(y));
	}

	inline void set(Type x, Type y) { 
		this->x = x;
		this->y = y;
	}

public:
	inline void operator=(const PoinT& point) { 
		x = point.x; 
		y = point.y; 
	}

	inline bool operator==(const PoinT& point) const { 
		return (x == point.x && y == point.y); 
	}

	inline bool operator!=(const PoinT& point) const { 
		return (x != point.x || y != point.y); 
	}

	inline PoinT operator+(const PoinT& point) const { 
		return PoinT(x + point.x, y + point.y); 
	}

	inline PoinT operator-(const PoinT& point) const { 
		return PoinT(x - point.x, y - point.y); 
	}

	inline PoinT& operator+=(const PoinT& point) {
		x += point.x;
		y += point.y;
		return *this;
	}

	inline PoinT& operator-=(const PoinT& point) {
		x -= point.x;
		y -= point.y;
		return *this;
	}

	template <class T>
	inline PoinT operator+(const T& value) const { 
		return PoinT(x + value, y + value); 
	}
	
	template <class T>
	inline PoinT operator-(const T& value) const { 
		return PoinT(x - value, y - value); 
	}

	template <class T>
	inline PoinT& operator+=(const T& value) {
		x += value;
		y += value;
		return *this;
	}

	template <class T>
	inline PoinT& operator-=(const T& value) {
		x -= value;
		y -= value;
		return *this;
	}

public:
	Type x;
	Type y;
};

typedef PoinT<s8> PointC;
typedef PoinT<u8> PointUC;
typedef PoinT<s16> PointSI;
typedef PoinT<u16> PointUSI;
typedef PoinT<s32> PointI;
typedef PoinT<u32> PointUI;
typedef PoinT<float> PointF;
typedef PoinT<double> PointD;
typedef PointI Point;
typedef PointF Vector2;
typedef Vector2 Vector2D;
