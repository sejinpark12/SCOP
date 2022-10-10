# 텍스처 입히기

## 텍스처의 필요성

## 텍스처의 의미

### 텍스처의 사용법

텍스처는 동일한 포맷의 이미지를 하나 이상 가지고 있는 OpenGL ES 오브젝트입니다.
텍스처는 아래의 두 가지 방식으로 사용할 수 있습니다.

1. 셰이더에서 텍스처에 접근하여 사용.
2. 렌더 타겟으로 사용.

### 텍스처의 제약조건

위에서 텍스처는 하나 이상의 이미지가 담긴 컨테이너라고 했습니다. 하지만 텍스처가 임의의 이미지를 저장하지는 않습니다. 텍스처에는 저장할 수 있는 이미지에 대한 세 가지 제약 조건이 존재합니다.

1. 텍스처 타입 : 텍스처에서 이미지의 배열을 정의
2. 텍스처 사이즈 : 텍스처에서 이미지의 크기를 정의
3. 이미지 포멧 : 이러한 모든 이미지가 공유하는 포멧을 정의

![texture](./images/Anatomy_of_a_Texture.png)

## 텍스처 오브젝트 생성

```c
void glGenTextures(GLsizei n, GLuint *textures);
```

이전의 다른 오브젝트들과 마찬가지로 텍스처도 `glGenTextures` 함수를 호출하여 오브젝트를 생성합니다.

## 텍스처 바인딩

```c
void glBindTexture(GLenum target, GLuint texture);
```

이전의 다른 오브젝트들과 마찬가지로 텍스처도 `glBindTexture` 함수를 호출하여 오브젝트를 바인딩합니다.

## 텍스처 할당

```c
void glTexImage2d(GLenum target,
                  GLint level,
                  GLint internalFormat,
                  GLsizei width,
                  GLsizei height,
                  GLint border,
                  GLenum format,
                  GLenum type,
                  const void *data);
```

## 텍스처 필터 설정

```c
void glTexParameteri(GLenum target, GLenum pname, GLint param);
```

## 유니폼 변수 설정

```c
GLint glGetUniformLocation(GLuint program, const GLchar *name);
```

```c
void glUniform1i(GLint location, GLint v0);
```

## 텍스처 활성화

```c
void glActiveTexture(GLenum texture);
```

## 텍스처 파괴

```c
void glDeleteTextures(GLsizei n, const GLuint *textures);
```

## 사각형 그리기

## 결과

## 참고

- [OpenGLES - GraphicsKorea](https://github.com/GraphicsKorea/OpenGLES)
- [Texture - OpenGL Wiki](https://www.khronos.org/opengl/wiki/Texture)
