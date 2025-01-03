/*
 * Nuklear - 1.32.0 - public domain
 * no warrenty implied; use at your own risk.
 */
/*
 * ==============================================================
 *
 *                              API
 *
 * ===============================================================
 */
#ifndef NK_WIN32_GL2_H_
#define NK_WIN32_GL2_H_

#define WIN32_LEAN_AND_MEAN
#include<windows.h>

#include<stddef.h>
#include<stdlib.h>

//#include"lefa/3rdparty/glad/OpenGL/glad_gl.h"
//#include"3rdparty/nuklear/nuklear.h"      /* Nuklear Gui LIB */
//#define GL2_0
//#include"lefa/contents/gl_manager.h"  /* LEFA ENGINE OpenGL Config Loader */
#include"nuklear_internal.h"          /* Nuklear headers */

//#include"lefa/platforms/windows_header.h" /* LEFA ENGINE Windows NT Config */

/*
NK_API struct nk_context*   nk_wgl2_init(HWND *win);
NK_API void                 nk_wgl2_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_wgl2_font_stash_end(void);
NK_API int                  nk_wgl2_handle_event(SDL_Event *evt);
NK_API void                 nk_wgl2_render(enum nk_anti_aliasing);
NK_API void                 nk_wgl2_shutdown(void);
*/

NK_API struct nk_context*   nk_wgl2_init(HWND *win);
NK_API void                 nk_wgl2_font_stash_begin(struct nk_font_atlas **atlas);
NK_API void                 nk_wgl2_font_stash_end(void);
NK_API void                 nk_wgl2_render(enum nk_anti_aliasing);
NK_API void                 nk_wgl2_shutdown(void);

#endif
/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef NK_WIN32_GL2_IMPLEMENTATION

struct nk_wgl2_device 
{
    struct nk_buffer cmds;
    struct nk_draw_null_texture null;
    GLuint font_tex;
};

struct nk_wgl2_vertex 
{
    float position[2];
    float uv[2];
    nk_byte col[4];
};

static struct nk_wgl2 
{
    HWND *win;
    struct nk_wgl2_device ogl;
    struct nk_context ctx;
    struct nk_font_atlas atlas;
} wgl2;




char* GetClipboardText() 
{
    char *text = NULL;
    if(OpenClipboard(NULL)) 
    {
        HANDLE hData = GetClipboardData(CF_TEXT);
        if(hData != NULL) 
        {
            char *buffer = (char*)GlobalLock(hData);
            if (buffer != NULL) 
            {
                text = strdup(buffer); // Ou outra função para copiar a string
                GlobalUnlock(hData);
            }
        }
        CloseClipboard();
    }
    return text;
}

void SetClipboardText(const char* text) 
{
    HGLOBAL hMem;

    if (OpenClipboard(NULL)) 
    {
        EmptyClipboard();
        hMem = GlobalAlloc(GMEM_MOVEABLE, strlen(text) + 1);
        if(hMem != NULL) 
        {
            char* memData = (char*)GlobalLock(hMem);
            strcpy(memData, text);
            GlobalUnlock(hMem);
            SetClipboardData(CF_TEXT, hMem);
        }
        CloseClipboard();
    }
}

void SetRelativeMouseMode(HWND hwnd, BOOL enable) 
{
    if(enable) 
    {
        // Esconder o cursor do mouse
        ShowCursor(FALSE);

        // Habilitar o modo de movimento relativo
        SetCapture(hwnd);
        //SetCursorPos(hwnd, 0, 0);
    } 
    else 
    {
        // Mostrar o cursor do mouse
        ShowCursor(TRUE);

        // Desabilitar o modo de movimento relativo
        ReleaseCapture();
    }
}

void MoveMouseToPosition(int x, int y) 
{
    SetCursorPos(x, y);
}


NK_INTERN void nk_wgl2_device_upload_atlas(const void *image, 
                                           int width, 
                                           int height)
{
    struct nk_wgl2_device *dev = &wgl2.ogl;
    glGenTextures(1, &dev->font_tex);
    glBindTexture(GL_TEXTURE_2D, 
                  dev->font_tex);

    glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MIN_FILTER, 
                    GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, 
                    GL_TEXTURE_MAG_FILTER, 
                    GL_LINEAR);

    glTexImage2D(GL_TEXTURE_2D, 
                 0, 
                 GL_RGBA, 
                 (GLsizei)width, 
                 (GLsizei)height, 
                 0,
                 GL_RGBA, 
                 GL_UNSIGNED_BYTE, 
                 image);
}

NK_API void nk_wgl2_render(enum nk_anti_aliasing AA)
{
    /* setup global state */
    struct nk_wgl2_device *dev = &wgl2.ogl;
    
    int width, 
        height;

    int display_width, 
        display_height;

    struct nk_vec2 scale;


    RECT rect;
    HWND hwnd = GetActiveWindow(); // Obter o identificador da janela ativa

    /* Obter as dimensões da janela */
    GetClientRect(hwnd, &rect);
    width = rect.right - rect.left;
    height = rect.bottom - rect.top;


    display_width = width;
    display_height = height;


    /*SDL_GetWindowSize(wgl2.win, &width, &height);
    */

    /*SDL_GL_GetDrawableSize(wgl2.win, 
                           &display_width, 
                           &display_height);
*/

    scale.x = (float)display_width/(float)width;
    scale.y = (float)display_height/(float)height;

    glPushAttrib(GL_ENABLE_BIT|GL_COLOR_BUFFER_BIT | GL_TRANSFORM_BIT);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_SCISSOR_TEST);
    glEnable(GL_BLEND);
    glEnable(GL_TEXTURE_2D);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /* setup viewport/project */
    glViewport(0,
               0,
               (GLsizei)display_width,
               (GLsizei)display_height);

    glMatrixMode(GL_PROJECTION);
    glPushMatrix();
    glLoadIdentity();

    glOrtho(0.0f, 
            width, 
            height, 
            0.0f, 
            -1.0f, 
            1.0f);

    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    glLoadIdentity();

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    {
        GLsizei vs = sizeof(struct nk_wgl2_vertex);
        size_t vp = offsetof(struct nk_wgl2_vertex, position);
        size_t vt = offsetof(struct nk_wgl2_vertex, uv);
        size_t vc = offsetof(struct nk_wgl2_vertex, col);

        /* convert from command queue into draw list and draw to screen */
        const struct nk_draw_command *cmd;
        const nk_draw_index *offset = NULL;
        struct nk_buffer vbuf, ebuf;

        /* fill converting configuration */
        struct nk_convert_config config;
        static const struct nk_draw_vertex_layout_element vertex_layout[] 
            = {
                {
                    NK_VERTEX_POSITION, 
                    NK_FORMAT_FLOAT, 
                    NK_OFFSETOF(struct nk_wgl2_vertex, position)},
                {
                    NK_VERTEX_TEXCOORD, 
                    NK_FORMAT_FLOAT, 
                    NK_OFFSETOF(struct nk_wgl2_vertex, uv)},
                {
                    NK_VERTEX_COLOR, 
                    NK_FORMAT_R8G8B8A8, 
                    NK_OFFSETOF(struct nk_wgl2_vertex, col)},
                {
                    NK_VERTEX_LAYOUT_END}
        };
        NK_MEMSET(&config, 0, sizeof(config));
        config.vertex_layout = vertex_layout;
        config.vertex_size = sizeof(struct nk_wgl2_vertex);
        config.vertex_alignment = NK_ALIGNOF(struct nk_wgl2_vertex);
        config.null = dev->null;
        config.circle_segment_count = 22;
        config.curve_segment_count = 22;
        config.arc_segment_count = 22;
        config.global_alpha = 1.0f;
        config.shape_AA = AA;
        config.line_AA = AA;

        /* convert shapes into vertexes */
        nk_buffer_init_default(&vbuf);
        nk_buffer_init_default(&ebuf);
        nk_convert(&wgl2.ctx, &dev->cmds, &vbuf, &ebuf, &config);

        /* setup vertex buffer pointer */
        {
            const void *vertices = nk_buffer_memory_const(&vbuf);
            glVertexPointer(2, GL_FLOAT, vs, (const void*)((const nk_byte*)vertices + vp));
            glTexCoordPointer(2, GL_FLOAT, vs, (const void*)((const nk_byte*)vertices + vt));
            glColorPointer(4, GL_UNSIGNED_BYTE, vs, (const void*)((const nk_byte*)vertices + vc));
        }

        /* iterate over and execute each draw command */
        offset = (const nk_draw_index*)nk_buffer_memory_const(&ebuf);

        nk_draw_foreach(cmd, 
                        &wgl2.ctx, 
                        &dev->cmds)
        {
            if (!cmd->elem_count) continue;
            glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
            glScissor(
                      (GLint)(cmd->clip_rect.x * scale.x),
                      (GLint)((height - (GLint)(cmd->clip_rect.y + cmd->clip_rect.h)) * scale.y),
                      (GLint)(cmd->clip_rect.w * scale.x),
                      (GLint)(cmd->clip_rect.h * scale.y));

            glDrawElements(GL_TRIANGLES, 
                          (GLsizei)cmd->elem_count, 
                           GL_UNSIGNED_SHORT, 
                           offset);

            offset += cmd->elem_count;
        }
        nk_clear(&wgl2.ctx);
        nk_buffer_free(&vbuf);
        nk_buffer_free(&ebuf);
    }

    /* default OpenGL state */
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_SCISSOR_TEST);
    glDisable(GL_BLEND);
    glDisable(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
    glMatrixMode(GL_MODELVIEW);
    glPopMatrix();
    glMatrixMode(GL_PROJECTION);
    glPopMatrix();
    glPopAttrib();
}

static void nk_wgl2_clipboard_paste(nk_handle usr, 
                                    struct nk_text_edit *edit)
{
    const char *text = GetClipboardText();
    if(text)
        nk_textedit_paste(edit, 
                          text, 
                          nk_strlen(text));
    (void)usr;
}

static void nk_wgl2_clipboard_copy(nk_handle usr, 
                                   const char *text, 
                                   int len)
{
    char *str = 0;

    (void)usr;

    if(!len) return;
            str = (char*)malloc((size_t)len+1);
    if (!str) return;
    memcpy(str, text, (size_t)len);
    str[len] = '\0';
    SetClipboardText(str);
    free(str);
}

NK_API struct nk_context* nk_wgl2_init(HWND *win)
{
    wgl2.win = win;
    nk_init_default(&wgl2.ctx, 0);
    wgl2.ctx.clip.copy = nk_wgl2_clipboard_copy;
    wgl2.ctx.clip.paste = nk_wgl2_clipboard_paste;
    wgl2.ctx.clip.userdata = nk_handle_ptr(0);
    nk_buffer_init_default(&wgl2.ogl.cmds);
    return &wgl2.ctx;
}

NK_API void nk_wgl2_font_stash_begin(struct nk_font_atlas **atlas)
{
    nk_font_atlas_init_default(&wgl2.atlas);
    nk_font_atlas_begin(&wgl2.atlas);
    *atlas = &wgl2.atlas;
}

NK_API void nk_wgl2_font_stash_end(void)
{
    const void *image; 
    int w, 
        h;
    image = nk_font_atlas_bake(&wgl2.atlas, 
                               &w, 
                               &h, 
                               NK_FONT_ATLAS_RGBA32);
    nk_wgl2_device_upload_atlas(image, w, h);
    nk_font_atlas_end(&wgl2.atlas, 
                      nk_handle_id((int)wgl2.ogl.font_tex), 
                      &wgl2.ogl.null);
    if(wgl2.atlas.default_font)
        nk_style_set_font(&wgl2.ctx, 
                          &wgl2.atlas.default_font->handle);
}


/*
NK_API int nk_wgl2_handle_event(MSG *evt)
{
    int x, y;
    int down;
    nk_glyph glyph;
    struct nk_context *ctx = &wgl2.ctx;

    //* optional grabbing behavior *
    if (ctx->input.mouse.grab) 
    {
        SetRelativeMouseMode(0,1);
        ctx->input.mouse.grab = 0;
    } 
    else if (ctx->input.mouse.ungrab) 
    {
        x = (int)ctx->input.mouse.prev.x, 
        y = (int)ctx->input.mouse.prev.y;

        SetRelativeMouseMode(0,0);
        //SDL_WarpMouseInWindow(wgl2.win, x, y);
        MoveMouseToPosition(x , y);
        ctx->input.mouse.ungrab = 0;
    }
     // Tratamento de eventos do mouse
    if (evt->message == WM_MOUSEMOVE) 
    {
        x = GET_X_LPARAM(evt->lParam);
        y = GET_Y_LPARAM(evt->lParam);
        nk_input_motion(ctx, x, y);
    } 
    else if (evt->message == WM_LBUTTONDOWN || evt->message == WM_LBUTTONUP) 
    {
        down = (evt->message == WM_LBUTTONDOWN) ? 1 : 0;
        x = GET_X_LPARAM(evt->lParam);
        y = GET_Y_LPARAM(evt->lParam);
        nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
    }




    





    return 0;
}
*/
#define GET_X_LPARAM(lp) ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp) ((int)(short)HIWORD(lp))

#ifndef WM_MOUSEWHEEL
    #define WM_MOUSEWHEEL 0x020A
#endif
#ifndef GET_WHEEL_DELTA_WPARAM
    #define GET_WHEEL_DELTA_WPARAM(wParam) ((short)HIWORD(wParam))
#endif
#ifndef WHEEL_DELTA
    #define WHEEL_DELTA 120
#endif

/*
void SetRelativeMouseMode(HWND hwnd, BOOL enable) 
{
    if(enable) 
    {
        ShowCursor(FALSE);
        SetCapture(hwnd);
    } 
    else 
    {
        ShowCursor(TRUE);
        ReleaseCapture();
    }
}
void MoveMouseToPosition(int x, int y) 
{
    SetCursorPos(x, y);
}
*/
/* Implementação da função para lidar com eventos de mouse */
/*NK_API int nk_wgl2_handle_mouse_event(MouseEvent *evt)
{
    struct nk_context *ctx = &wgl2.ogl;

    if()


}
*/


/* 
NK_API int nk_sdl_handle_mouse_event(SDL_Event *evt) 
{
    struct nk_context *ctx = &sdl.ctx;

    if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP) 
    {
        /* mouse button *
        int down = evt->type == SDL_MOUSEBUTTONDOWN;
        const int x = evt->button.x, y = evt->button.y;
        
        if (evt->button.button == SDL_BUTTON_LEFT) 
        {
            if (evt->button.clicks > 1)
            {
                nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
                nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
            } 
        }
        else if (evt->button.button == SDL_BUTTON_MIDDLE)
        {
            nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
        }
        else if (evt->button.button == SDL_BUTTON_RIGHT)
        {
            nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
        }
        return 1;
    } 
    else if (evt->type == SDL_MOUSEMOTION) 
    {
        /* mouse motion *
        if (ctx->input.mouse.grabbed) 
        {
            int x = (int)ctx->input.mouse.prev.x, y = (int)ctx->input.mouse.prev.y;
            nk_input_motion(ctx, x + evt->motion.xrel, y + evt->motion.yrel);
        } 
        else nk_input_motion(ctx, evt->motion.x, evt->motion.y);
        return 1;
    } 
    else if (evt->type == SDL_MOUSEWHEEL) 
    {
        /* mouse wheel *
        nk_input_scroll(ctx,nk_vec2((float)evt->wheel.x,(float)evt->wheel.y));
        return 1;
    }
    return 0;
}
 */

/*
NK_API int nk_sdl_handle_mouse_event(SDL_Event *evt) 
{
    struct nk_context *ctx = &sdl.ctx;

    if (evt->type == SDL_MOUSEBUTTONDOWN || evt->type == SDL_MOUSEBUTTONUP) 
    {
        /* mouse button *
        int down = evt->type == SDL_MOUSEBUTTONDOWN;
        const int x = evt->button.x, 
                  y = evt->button.y;

        if (evt->button.button == SDL_BUTTON_LEFT) 
        {
            if (evt->button.clicks > 1)
            {
                nk_input_button(ctx, NK_BUTTON_DOUBLE, x, y, down);
                nk_input_button(ctx, NK_BUTTON_LEFT, x, y, down);
            }
        }
        else if (evt->button.button == SDL_BUTTON_MIDDLE)
            nk_input_button(ctx, NK_BUTTON_MIDDLE, x, y, down);
        else if (evt->button.button == SDL_BUTTON_RIGHT)
            nk_input_button(ctx, NK_BUTTON_RIGHT, x, y, down);
        return 1;
    } 
    else if (evt->type == SDL_MOUSEMOTION) 
    {
        /* mouse motion *
        if (ctx->input.mouse.grabbed) 
        {
            int x = (int)ctx->input.mouse.prev.x, 
                y = (int)ctx->input.mouse.prev.y;

            nk_input_motion(ctx, x + evt->motion.xrel, 
                                 y + evt->motion.yrel);
        } 
        else nk_input_motion(ctx, evt->motion.x, evt->motion.y);
        return 1;
    } 
    else if (evt->type == SDL_MOUSEWHEEL) 
    {
        /* mouse wheel *
        nk_input_scroll(ctx,nk_vec2((float)evt->wheel.x,
                                    (float)evt->wheel.y));
        return 1;
    }
    return 0;
} */


NK_API void nk_wgl2_shutdown(void)
{
    struct nk_wgl2_device *dev = &wgl2.ogl;
    nk_font_atlas_clear(&wgl2.atlas);
    nk_free(&wgl2.ctx);
    glDeleteTextures(1, &dev->font_tex);
    nk_buffer_free(&dev->cmds);
    memset(&wgl2, 0, sizeof(wgl2));
}

#endif
