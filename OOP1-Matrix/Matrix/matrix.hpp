//  matrix.hpp
//  Created by Arya on 2018/1/5.

#ifndef SJTU_MATRIX_HPP
#define SJTU_MATRIX_HPP
#include <algorithm>

namespace sjtu
{
	template <class T>
	class Matrix
	{

	private:
		size_t row_size;
		size_t col_size;
		T* data = nullptr;

	public:
		const T &operator()(size_t i, size_t j) const
		{
			if(!(i >= 0 && i < row_size && j >= 0 && j < col_size))
				throw std::invalid_argument("Out of range");
			return data[col_size * i + j];
		}

		T &operator()(size_t i, size_t j)
		{
			if(!(i >= 0 && i < row_size && j >= 0 && j < col_size))
				throw std::invalid_argument("Out of range");
			return data[col_size * i + j];
		}
	public:
		Matrix() = default;

		Matrix(size_t n, size_t m, T _init = T()):row_size(n), col_size(m)
		{
			data = new T[row_size * col_size];
			for(size_t i = 0; i < row_size * col_size; i++)
				data[i] = _init;
		}

		explicit Matrix(std::pair<size_t, size_t> sz, T _init = T()):row_size(sz.first), col_size(sz.second)
		{
			data = new T[row_size * col_size];
			for(size_t i = 0; i < row_size * col_size; i++)
				data[i] = _init;
		}

		Matrix(const Matrix &o)
		{
			row_size = o.row_size;
			col_size = o.col_size;
			data = new T[row_size * col_size];
			for(size_t i = 0; i < row_size * col_size; i++)
				data[i] = o.data[i];
		}

		template <class U>
		Matrix(const Matrix<U> &o)
		{
			row_size = o.rowLength();
			col_size = o.columnLength();
			data = new T[row_size * col_size];
			for(size_t i = 0; i < row_size; i++)
				for(size_t j = 0; j < col_size; j++)
					data[col_size * i + j] = (T)o(i, j);
		}

		Matrix &operator=(const Matrix &o)
		{
			if(this == &o)
				return *this;
            if(data != nullptr)
                delete [] data;
			row_size = o.rowLength();
			col_size = o.columnLength();
			data = new T[row_size * col_size];
			for(size_t i = 0; i < row_size * col_size; i++)
				data[i] = o.data[i];
			return *this;
		}

		template <class U>
		Matrix &operator=(const Matrix<U> &o)
		{
			if((void*)this == (void*)&o)
				return *this;
            if(data != nullptr)
                delete [] data;
			row_size = o.rowLength();
			col_size = o.columnLength();
			data = new T[row_size * col_size];
			for(size_t i = 0; i < row_size; i++)
				for(size_t j = 0; j < col_size; j++)
					data[col_size * i + j] = (T)o(i, j);
			return *this;
		}

		Matrix(Matrix &&o) noexcept: row_size(o.row_size), col_size(o.col_size), data(o.data)
        {
            o.data = nullptr;
        }
        //row_size = std::move(o.row_size)

		Matrix &operator=(Matrix &&o) noexcept
		{
			if(this == &o)
				return *this;
			if(data != nullptr)
                delete [] data;
			row_size = o.row_size;
			col_size = o.col_size;
			data = o.data;
			o.data = nullptr;
			return *this;
		}

		~Matrix()
		{
            if(data != nullptr)
            {
                delete [] data;
                data = nullptr;
            }
		}

		Matrix(std::initializer_list<std::initializer_list<T>> il)
		{
			bool flag = false;
			row_size = il.size();
			for(auto i:il)
			{
				if(!flag)
				{
					col_size = i.size();
					flag = true;
				}
				else if(col_size != i.size())
					throw std::invalid_argument("invalid initializer_list");
			}
			data = new T[row_size * col_size];
			size_t tmp = 0;
			for(auto i:il)
				for(auto j:i)
					data[tmp++] = j;
		}

	public:
		size_t rowLength() const {return row_size;}

		size_t columnLength() const {return col_size;}

		void resize(size_t _n, size_t _m, T _init = T())
		{
			if(_n * _m != row_size * col_size)
			{
                T *tmp = new T[_n * _m];
                for(size_t i = 0; i < std::min(_n * _m, row_size * col_size); i++)
                    tmp[i] = data[i];
                for(size_t i = row_size * col_size; i < _n * _m; i++)
                    tmp[i] = _init;
                delete [] data;
                data = tmp;
			}
			row_size = _n;
			col_size = _m;
		}

		void resize(std::pair<size_t, size_t> sz, T _init = T())
		{
			resize(sz.first, sz.second, _init);
		}

		std::pair<size_t, size_t> size() const
		{
			return std::make_pair(row_size, col_size);
		};

		void clear()
		{
			row_size = col_size = 0;
			this->~Matrix();
		}

	public:

		Matrix<T> row(size_t i) const
		{
			if(!(i >= 0 && i < row_size))
				throw std::invalid_argument("Out of range");
			Matrix<T> tmp(1, col_size);
			for(size_t j = 0; j < col_size; j++)
                tmp(0, j) = (*this)(i, j);
			return tmp;
		}

		Matrix<T> column(size_t i) const
		{
			if(!(i >= 0 && i < col_size))
				throw std::invalid_argument("Out of range");
			Matrix<T> tmp(row_size, 1);
			for(size_t j = 0; j < row_size; j++)
				tmp(j, 0) = (*this)(j, i);
			return tmp;
		}


	public:

        template <class U>
        bool sameSize(const Matrix<U> &o) const
        {
            return row_size == o.rowLength() && col_size == o.columnLength();
        }

		template <class U>
		bool operator==(const Matrix<U> &o) const
		{
			if(!sameSize(o))
                return false;
			for(size_t i = 0; i < row_size; i++)
				for(size_t j = 0; j < col_size; j++)
					if((*this)(i, j) != o(i, j))
						return false;
			return true;
		}

		template <class U>
		bool operator!=(const Matrix<U> &o) const
		{
			return !(*this == o);
		}

		Matrix operator-() const
		{
			for(size_t i = 0; i < row_size * col_size; i++)
				data[i] = -data[i];
            return *this;
		}

		template <class U>
		Matrix &operator+=(const Matrix<U> &o)
		{
            if(!sameSize(o))
                throw std::invalid_argument("Size cannot match");
			for(size_t i = 0; i < row_size; i++)
				for(size_t j = 0; j < col_size; j++)
					(*this)(i, j) += (T)(o(i, j));
			return *this;
		}

		template <class U>
		Matrix &operator-=(const Matrix<U> &o)
		{
            if(!sameSize(o))
                throw std::invalid_argument("Size cannot match");
			for(size_t i = 0; i < row_size; i++)
				for(size_t j = 0; j < col_size; j++)
					(*this)(i, j) -= (T)(o(i, j));
			return *this;
		}

		template <class U>
		Matrix &operator*=(const U &x)
		{
			for(size_t i = 0; i < row_size * col_size; i++)
				data[i] *= (T)x;
			return *this;
		}

		Matrix tran() const
		{
			Matrix tmp(col_size, row_size);
			size_t pos = 0;
			for(size_t j = 0; j < col_size; j++)
				for(size_t i = 0; i < row_size; i++)
					tmp.data[pos++] = (*this)(i, j);
			return tmp;
		}

	using pii = std::pair<size_t, size_t>;

	public: // iterator
		class iterator
		{
			friend class Matrix <T>;
		public:
			using iterator_category = std::random_access_iterator_tag;
			using value_type        = T;
			using pointer           = T *;
			using reference         = T &;
			using size_type         = size_t;
			using difference_type   = std::ptrdiff_t;

			//iterator() = default;

			iterator(const iterator &) = default;

			iterator &operator=(const iterator &) = default;

		private:
			difference_type pos;
			Matrix <T> *p;
			pii lc, rc;

		public:

            iterator(difference_type pos = -1, Matrix <T> *pt = nullptr, pii lc = std::make_pair(T(), T()), pii rc = std::make_pair(T(), T())): pos(pos), p(pt), lc(lc), rc(rc) {}
			
			difference_type operator-(const iterator &o)
			{
				return pos - o.pos;
			}

			iterator &operator+=(difference_type offset)
			{
				if(pos + offset > (p->rowLength()) * (p->columnLength()))
					throw std::invalid_argument("Out of Range");
				pos += offset;
				return *this;
			}

			iterator operator+(difference_type offset) const
			{
				if(pos + offset > (p->rowLength()) * (p->columnLength()))
					throw std::invalid_argument("Out of Range");
				return iterator(pos + offset, p, lc, rc);
			}

			iterator &operator-=(difference_type offset)
			{
				if(pos - offset < 0)
					throw std::invalid_argument("Out of Range");
				pos -= offset;
				return *this;
			}

			iterator operator-(difference_type offset) const
			{
				if(pos - offset < 0)
					throw std::invalid_argument("Out of Range");
				return iterator(pos - offset, p, lc, rc);
			}

			iterator &operator++()
			{
				return (*this) += 1;
			}

			iterator operator++(int)
			{
				if(pos + 1 > (p->rowLength()) * (p->columnLength()))
					throw std::invalid_argument("Out of Range");
				iterator tmp = *this;
				pos++;
				return tmp;
			}

			iterator &operator--()
			{
				return (*this) -= 1;
			}

			iterator operator--(int)
			{
				if(pos - 1 < 0)
					throw std::invalid_argument("Out of Range");
				iterator tmp = *this;
				pos--;
				return tmp;
			}

			reference operator*() const
			{
				size_type col = rc.second - lc.second + 1;
				size_type r = pos / col, c = pos - r * col;
				return (*p)(r + lc.first, c + lc.second);
			}

			pointer operator->() const
			{
				size_type col = rc.second - lc.second + 1;
				size_type r = pos / col, c = pos - r * col;
				return &(*p)(r + lc.first, c + lc.second);
			}

			bool operator==(const iterator &o) const
			{
				return pos == o.pos && p == o.p && lc == o.lc && rc == o.rc;
			}

			bool operator!=(const iterator &o) const
			{
				return pos != o.pos || p != o.p || lc != o.lc || rc != o.rc;
			}
		};

		iterator begin()
		{
			return iterator(0, this, std::make_pair(0, 0), std::make_pair(row_size - 1, col_size - 1));
		}

		iterator end()
		{
			return iterator((row_size * col_size), this, std::make_pair(0, 0), std::make_pair(row_size - 1, col_size - 1));
		}

		std::pair<iterator, iterator> subMatrix(std::pair<size_t, size_t> l, std::pair<size_t, size_t> r)
		{
			if(l.first > r.first || l.second < r.second)
				throw std::invalid_argument("invalid submatrix");
            if(l.first < 0 || r.first >= row_size || l.second < 0 || r.second >= col_size)
                throw std::invalid_argument("Out of range");
			return std::make_pair(iterator(0, this, l, r), iterator((r.first - l.first + 1) * (r.second - l.second + 1), this, l, r));
		};
    };
}

//
namespace sjtu
{
	template <class T, class U>
	auto operator*(const Matrix<T> &mat, const U &x)
	{
		Matrix<decltype(T() * U())> tmp(mat);
		tmp *= x;
		return tmp;
	};

	template <class T, class U>
	auto operator*(const U &x, const Matrix<T> &mat)
	{
		return mat * x;
	};

	template <class U, class V>
	auto operator*(const Matrix<U> &a, const Matrix<V> &b)
	{
		size_t row_size = a.rowLength(), col_size = b.columnLength();
		size_t mid = a.columnLength();
		if(mid != b.rowLength())
			throw std::invalid_argument("Size cannot match");

		Matrix<decltype(U() * V())> tmp(row_size, col_size);
		for(size_t k = 0; k < mid; k++)
			for(size_t i = 0; i < row_size; i++)
				for(size_t j = 0; j < col_size; j++)
					tmp(i, j) += a(i, k) * b(k, j);
		return tmp;
	};

	template <class U, class V>
	auto operator+(const Matrix<U> &a, const Matrix<V> &b)
	{
        if(!a.sameSize(b))
            throw std::invalid_argument("Size cannot match");
		Matrix<decltype(U() * V())> tmp(a);
		tmp += b;
		return tmp;
	};

	template <class U, class V>
	auto operator-(const Matrix<U> &a, const Matrix<V> &b)
	{
        if(!a.sameSize(b))
            throw std::invalid_argument("Size cannot match");
		Matrix<decltype(U() * V())> tmp(a);
		tmp -= b;
		return tmp;
	};

}

#endif //SJTU_MATRIX_HPP
