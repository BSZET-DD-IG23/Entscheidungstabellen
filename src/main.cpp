#include <raylib.h>
#include <raymath.h>

#include <nlohmann/json.hpp>

using json = nlohmann::json;

#include <vector>
#include <string>
#include <sol/sol.hpp>

#define COLOR_ORANGE (Color) {225, 138, 50, 255}
#define COLOR_BLUE (Color) {111, 173, 162, 255}

struct Error {
    enum ErrorCode {
        SUCCESS = 0,
        FAILURE = -1,
        RUNTIME_ERROR = -2,
        COMPILE_ERROR = -3,
        SYNTAX_ERROR = -4,
        SEMANTIC_ERROR = -5,
        INTERNAL_ERROR = -6,
    };

    std::string message;
    size_t code = SUCCESS;
};

template<typename T>
concept CArithmetic = std::is_arithmetic_v<T>;

template<CArithmetic T>
struct TVector2 {
	T x;
	T y;

	// Constructors
	TVector2() = default;
	explicit TVector2(T x, T y) : x(x), y(y) {}
	TVector2(const TVector2& other) : x(other.x), y(other.y) {}
	TVector2(TVector2&& other)  noexcept : x(other.x), y(other.y) {}
	explicit TVector2(Vector2 rl) : x(rl.x), y(rl.y) {}

	// Math
	TVector2<T> operator+(const TVector2<T>& other) const {
		return TVector2<T>(x + other.x, y + other.y);
	}
	TVector2<T> operator-(const TVector2<T>& other) const {
		return TVector2<T>(x - other.x, y - other.y);
	}
	TVector2<T> operator*(const TVector2<T>& other) const {
		return TVector2<T>(x * other.x, y * other.y);
	}
	TVector2<T> operator/(const TVector2<T>& other) const {
		return TVector2<T>(x / other.x, y / other.y);
	}

	// operators
	TVector2<T>& operator+=(const TVector2<T>& other) {
		x += other.x;
		y += other.y;
		return *this;
	}
	TVector2<T>& operator-=(const TVector2<T>& other) {
		x -= other.x;
		y -= other.y;
		return *this;
	}
	TVector2<T>& operator*=(const TVector2<T>& other) {
		x *= other.x;
		y *= other.y;
		return *this;
	}
	TVector2<T>& operator/=(const TVector2<T>& other) {
		x /= other.x;
		y /= other.y;
		return *this;
	}

	// Comparison
	bool operator==(const TVector2<T>& other) const {
		return x == other.x && y == other.y;
	}
	bool operator!=(const TVector2<T>& other) const {
		return x != other.x || y != other.y;
	}

	TVector2& operator()(T x,  T y) const {
		this->x = x;
		this->y = y;
		return *this;
	}

	// to Raylib
	explicit operator Vector2() const {
		return (Vector2){x, y};
	}
};

typedef TVector2<float> vec2, vec2f;

typedef TVector2<int> ivec2;
typedef TVector2<size_t> uvec2;
typedef TVector2<signed long long> svec2;
typedef TVector2<double> dvec2;

typedef TVector2<bool> bvec2;


class Table {
	Color bg_color = COLOR_BLUE;
	Color border_color = DARKGRAY;
	Color text_color = BLACK;
	Color selected_color = COLOR_ORANGE;

	Rectangle bounds;

	size_t rows = 5;
	size_t cols = 10;
	svec2 selected = svec2{0,0};
public:

    Table() = default;
	Table(Rectangle bounds, size_t rows, size_t cols) : bounds(bounds), rows(rows), cols(cols) {}

	void move_y(bool pos) {
		if (pos) {
			selected.y++;
		}
		else {
			selected.y--;
		}

		if (selected.y < 0) {
			selected.y = rows - 1;
		}
		else if (selected.y >= rows) {
			selected.y = 0;
		}
	}

	void move_x(bool pos) {
		if (pos) {
			selected.x++;
		}
		else {
			selected.x--;
		}

		if (selected.x < 0) {
			selected.x = 0;
			move_y(false);
		}
		else if (selected.x >= cols) {
			selected.x = 0;
			move_y(true);
		}
	}

	virtual void update() {
		if (IsKeyPressed(KEY_UP)) {
			move_y(false);
		}
		if (IsKeyPressed(KEY_DOWN)) {
			move_y(true);
		}
		if (IsKeyPressed(KEY_LEFT)) {
			move_x(false);
		}
		if (IsKeyPressed(KEY_RIGHT)) {
			move_x(true);
		}
	}


	virtual void draw() {
	    const float border_size = bounds.width / (1.2f* static_cast<float>(cols));
    	const float cell_width = bounds.width / static_cast<float>(cols);
    	const float cell_height = bounds.height / static_cast<float>(rows);

    	DrawRectangleRec(bounds, bg_color);

    	for (size_t i = 0; i < rows; i++) {
    		const float y = bounds.y + cell_height * static_cast<float>(i);
    		DrawLineEx(Vector2{bounds.x, y}, Vector2{bounds.x +  bounds.width, y}, border_size, border_color);
    	}

    	for (size_t i = 0; i < cols; i++) {
    		const float x = bounds.x + cell_width * (float)i;
    		DrawLineEx(Vector2{x, bounds.y}, Vector2{x, bounds.y + bounds.height}, border_size, border_color);
    	}

    	DrawRectangleLinesEx({bounds.x, bounds.y, bounds.width, bounds.height}, border_size, border_color);

    	if (selected.x > 0 && selected.y > 0) {
    		const float y = bounds.y + cell_height * static_cast<float>(selected.x);
    		const float x = bounds.x + cell_width * static_cast<float>(selected.y);
			DrawRectangleRounded({x,y, cell_width, cell_height}, border_size, 5, selected_color);
    	}
    }
};


int main() {
    InitWindow(900, 600, "Entscheidungstabellen");

    Table table({0,0,900,600}, 5, 10);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        {
        	table.update();
            table.draw();
        }
        EndDrawing();
    }
    return 0;
}
