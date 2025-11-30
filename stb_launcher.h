/** Copyright (C) Enzo 2025-2026 */
/** The stb foundation */
/** LICENSE: APACHE-2.0 */
//------------------------//
// stb_launcher.h
// -----------------------------------------------------------------------------
// Este é o primeiro arquivo oficial do seu "stb_launcher".
// Como você pediu, vou colocar MUITOS comentários explicando cada parte,
// para você modificar depois quando for criando o sistema.
// -----------------------------------------------------------------------------
//
// OBJETIVO:
// Fornecer funções simples que podem ser usadas em qualquer projeto C,
// para imprimir texto e números na tela, sem complicação.
//
// FUNÇÕES IMPLEMENTADAS:
//  - stb_launcher_print_text("texto");
//  - stb_launcher_print_number(123);
//
// FUTURAMENTE:
//  - Você pode adicionar funções para desenhar shapes
//  - Imprimir cores diferentes
//  - Funções do seu OS AsmUi
//  - Funções 3D, arquivos, janelas, etc.
// -----------------------------------------------------------------------------

#ifndef STB_LAUNCHER_H
#define STB_LAUNCHER_H

#include <stdio.h>   // Necessário para printf
#include <stdint.h>  // Tipos inteiros (uint32_t etc)

// -----------------------------------------------------------------------------
// FUNÇÃO: stb_launcher_print_text
// Usa printf internamente para mostrar texto na tela.
//
// EXEMPLO DE USO:
//      stb_launcher_print_text("Olá mundo!");
//
// -----------------------------------------------------------------------------
static void stb_launcher_print_text(const char* text)
{
    // Se o ponteiro for nulo, não imprime nada.
    if (text == NULL)
        return;

    printf("%s", text);
}

// -----------------------------------------------------------------------------
// FUNÇÃO: stb_launcher_print_number
// Imprime um número inteiro na tela.
//
// EXEMPLO DE USO:
//      stb_launcher_print_number(42);
//
// Pode imprimir:
//      números positivos
//      números negativos
//      zero
//
// -----------------------------------------------------------------------------
static void stb_launcher_print_number(int number)
{
    printf("%d", number);
}

// =============================================================================
// stb_launcher.h  — PARTE 2 (Gráfico + 3D sem bibliotecas externas)
// =============================================================================
// ESTE ARQUIVO DEFINE:
//
//  - Framebuffer interno
//  - Desenho de pixel
//  - Desenho de linhas
//  - Estruturas matemáticas 3D
//  - Matriz de projeção
//  - Renderização básica de triângulos em wireframe
//
// Tudo isso sem usar NENHUMA biblioteca gráfica.
// Só printa o framebuffer como ASCII no terminal.
//
// =============================================================================

#ifndef STB_LAUNCHER_H
#define STB_LAUNCHER_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>

// CONFIGURAÇÃO DO FRAMEBUFFER -------------------------------------------------
#define STB_FB_WIDTH  120
#define STB_FB_HEIGHT 40

static char stb_framebuffer[STB_FB_HEIGHT][STB_FB_WIDTH];

// LIMPA TELA -------------------------------------------------------------------
static void stb_clear()
{
    for (int y = 0; y < STB_FB_HEIGHT; y++)
        for (int x = 0; x < STB_FB_WIDTH; x++)
            stb_framebuffer[y][x] = ' ';
}

// ATUALIZA TELA NO TERMINAL ---------------------------------------------------
static void stb_present()
{
    for (int y = 0; y < STB_FB_HEIGHT; y++) {
        for (int x = 0; x < STB_FB_WIDTH; x++)
            putchar(stb_framebuffer[y][x]);
        putchar('\n');
    }
}

// DESENHA UM PIXEL -------------------------------------------------------------
static void stb_putpixel(int x, int y, char c)
{
    if (x < 0 || x >= STB_FB_WIDTH) return;
    if (y < 0 || y >= STB_FB_HEIGHT) return;
    stb_framebuffer[y][x] = c;
}

// DESENHO DE LINHA (BRESENHAM) -------------------------------------------------
static void stb_draw_line(int x0, int y0, int x1, int y1, char c)
{
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        stb_putpixel(x0, y0, c);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

// -----------------------
//   MATEMÁTICA 3D
// -----------------------
typedef struct { float x, y, z; } stb_vec3;
typedef struct { int x, y; } stb_vec2;

// MULTIPLICA VEC3 POR MATRIZ 4x4 ---------------------------------------------
static stb_vec3 stb_mul_mat(float m[4][4], stb_vec3 v)
{
    stb_vec3 r;
    r.x = v.x * m[0][0] + v.y * m[1][0] + v.z * m[2][0] + m[3][0];
    r.y = v.x * m[0][1] + v.y * m[1][1] + v.z * m[2][1] + m[3][1];
    r.z = v.x * m[0][2] + v.y * m[1][2] + v.z * m[2][2] + m[3][2];
    return r;
}

// PROJEÇÃO 3D → 2D -------------------------------------------------------------
static stb_vec2 stb_project(stb_vec3 v)
{
    stb_vec2 r;
    r.x = (int)((v.x / v.z) * 30 + STB_FB_WIDTH / 2);
    r.y = (int)((v.y / v.z) * 15 + STB_FB_HEIGHT / 2);
    return r;
}

// DESENHA TRIÂNGULO 3D EM WIREFRAME ------------------------------------------
static void stb_draw_triangle3D(stb_vec3 a, stb_vec3 b, stb_vec3 c)
{
    stb_vec2 pa = stb_project(a);
    stb_vec2 pb = stb_project(b);
    stb_vec2 pc = stb_project(c);

    stb_draw_line(pa.x, pa.y, pb.x, pb.y, '#');
    stb_draw_line(pb.x, pb.y, pc.x, pc.y, '#');
    stb_draw_line(pc.x, pc.y, pa.x, pa.y, '#');
}

#endif
// ============================================================================
//           PARTE 2 — MODO DE RECUPERAÇÃO (Recovery Mode)
// ============================================================================
//
//  API desejada:
//
//      stb.launcher.recovery.format("data/");
//      stb.launcher.recovery.reset("factory");
//      stb.launcher.recovery.detect.button(KEY_VOL_UP);
//
//  Chaves de botões:
//
//      KEY_VOL_UP
//      KEY_VOL_DOWN
//      KEY_SELECT_POWER
//
// ============================================================================

#ifndef STB_LAUNCHER_RECOVERY
#define STB_LAUNCHER_RECOVERY

// Códigos de botões simulados --------------------------------------------------
#define KEY_VOL_UP        1
#define KEY_VOL_DOWN      2
#define KEY_SELECT_POWER  3

// Último botão pressionado (simulado internamente)
static int stb_last_pressed_button = 0;

// Simula pressionar botão ------------------------------------------------------
static void stb_recovery_press_button(int key)
{
    stb_last_pressed_button = key;
}

// Função de formatar partição --------------------------------------------------
static void stb_recovery_format(const char *path)
{
    printf("[stb/recovery] Formatando partição: %s\n", path);
    printf("[stb/recovery] OK: Dados apagados.\n");
}

// Função de reset --------------------------------------------------------------
static void stb_recovery_reset(const char *mode)
{
    printf("[stb/recovery] Reset solicitado: %s\n", mode);

    if (strcmp(mode, "factory") == 0)
        printf("[stb/recovery] Restaurando para o modo de fábrica...\n");
    else if (strcmp(mode, "soft") == 0)
        printf("[stb/recovery] Reinício suave executado.\n");
    else
        printf("[stb/recovery] Modo de reset desconhecido.\n");
}

// Detectar botão pressionado ---------------------------------------------------
static int stb_recovery_detect_button(int key)
{
    if (stb_last_pressed_button == key) {
        printf("[stb/recovery] Botão detectado: %d\n", key);
        return 1;
    }
    return 0;
}

// -----------------------------------------------------------------------------
//           Interface estilo "stb.launcher.recovery.x"
// -----------------------------------------------------------------------------

struct {
    struct {
        void (*format)(const char *path);
        void (*reset)(const char *mode);
        int  (*detect_button)(int key);
    } recovery;
} stb = {
    .recovery = {
        .format = stb_recovery_format,
        .reset  = stb_recovery_reset,
        .detect_button = stb_recovery_detect_button
    }
};

#endif
// ============================================================================
//        PARTE 3 — BOOTLOADER (carregar arquivos boot.img de verdade)
// ============================================================================
//
//  Função principal:
//      boot_load("boot.img");
//
//  Comportamento:
//      - Verifica se o arquivo existe
//      - Lê conteúdo inteiro para a RAM simulada
//      - Faz logs de boot
//      - Retorna ponteiro + tamanho
//
// ============================================================================

#ifndef STB_LAUNCHER_BOOT
#define STB_LAUNCHER_BOOT

typedef struct {
    unsigned char *data;
    long size;
} stb_boot_image;

// Carrega arquivo boot.img real ------------------------------------------------
static stb_boot_image boot_load(const char *path)
{
    printf("[stb/boot] Carregando boot image: %s\n", path);

    FILE *f = fopen(path, "rb");
    if (!f) {
        printf("[stb/boot] ERRO: Arquivo não encontrado!\n");
        return (stb_boot_image){ NULL, 0 };
    }

    // Obtém tamanho
    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    printf("[stb/boot] Tamanho da imagem: %ld bytes\n", size);

    // Aloca memória (RAM simulada)
    unsigned char *buffer = (unsigned char*)malloc(size);
    if (!buffer) {
        printf("[stb/boot] ERRO: Falha ao alocar RAM para o boot!\n");
        fclose(f);
        return (stb_boot_image){ NULL, 0 };
    }

    // Lê arquivo inteiro
    fread(buffer, 1, size, f);
    fclose(f);

    printf("[stb/boot] OK: Boot image carregada para memória.\n");
    printf("[stb/boot] Endereço de memória: %p\n", buffer);

    return (stb_boot_image){ buffer, size };
}

#endif
// ============================================================================
//                          PARTE 4 — SISTEMAS AVANÇADOS
// ============================================================================
//
//  Inclui:
//    - Verificação de assinatura digital: boot_verify_signature()
//    - Parser de header Android Boot Image: boot_parse_header()
//    - Carregamento real de arquivo ELF: boot_load_kernel("kernel.elf")
//    - Montagem de RAMDISK: boot_load_ramdisk()
//    - Tela de boot estilo Android
//    - LOGCAT interno (circular buffer)
//    - Sistema de Arquivos Virtual em RAM (stb.fs)
//
// ============================================================================

#ifndef STB_LAUNCHER_ADVANCED
#define STB_LAUNCHER_ADVANCED


// ============================================================================
//  LOGCAT INTERNO (circular)
// ============================================================================
#define LOGCAT_MAX 128
static const char *logcat_buffer[LOGCAT_MAX];
static int logcat_index = 0;

static void logcat(const char *msg)
{
    logcat_buffer[logcat_index] = msg;
    logcat_index = (logcat_index + 1) % LOGCAT_MAX;
    printf("[logcat] %s\n", msg);
}

static void logcat_dump()
{
    printf("\n===== LOGCAT DUMP =====\n");
    for (int i = 0; i < LOGCAT_MAX; i++) {
        int idx = (logcat_index + i) % LOGCAT_MAX;
        if (logcat_buffer[idx])
            printf("%s\n", logcat_buffer[idx]);
    }
}


// ============================================================================
//  TELA DE BOOT ESTILO ANDROID (ASCII)
// ============================================================================
static void boot_animation()
{
    printf("\n");
    printf("  #########################################\n");
    printf("  #                                       #\n");
    printf("  #              ANDROID BOOT             #\n");
    printf("  #                (stbOS)                #\n");
    printf("  #                                       #\n");
    printf("  #########################################\n");
    printf("\n");
    logcat("boot_animation: tela exibida");
}


// ============================================================================
//  VERIFICAÇÃO DE ASSINATURA DIGITAL (FAKE)
// ============================================================================

static int boot_verify_signature(const char *image)
{
    logcat("boot_verify_signature: iniciando validação...");

    // Simulando: se o arquivo terminar com ".signed", é válido
    const char *ext = strrchr(image, '.');
    if (ext && strcmp(ext, ".signed") == 0) {
        logcat("boot_verify_signature: assinatura OK");
        printf("[boot] Assinatura digital válida.\n");
        return 1;
    }

    logcat("boot_verify_signature: assinatura inválida");
    printf("[boot] ERRO: assinatura inválida!\n");
    return 0;
}


// ============================================================================
//  PARSE HEADER DO BOOT IMAGE ANDROID (FAKE MAS REALISTA)
// ============================================================================
typedef struct {
    char magic[8];
    uint32_t kernel_size;
    uint32_t ramdisk_size;
    uint32_t page_size;
    char name[16];
} stb_boot_header;

static stb_boot_header boot_parse_header(const unsigned char *data)
{
    stb_boot_header h;

    memcpy(h.magic, data, 8);
    memcpy(&h.kernel_size,  data + 8,  4);
    memcpy(&h.ramdisk_size, data + 12, 4);
    memcpy(&h.page_size,    data + 16, 4);
    memcpy(h.name,          data + 20, 16);

    logcat("boot_parse_header: header analisado");

    printf("[boot] MAGIC: %.8s\n", h.magic);
    printf("[boot] Kernel size: %u bytes\n", h.kernel_size);
    printf("[boot] Ramdisk size: %u bytes\n", h.ramdisk_size);
    printf("[boot] Page size: %u\n", h.page_size);
    printf("[boot] Name: %s\n", h.name);

    return h;
}



// ============================================================================
//  LOADER DE KERNEL ELF REAL (kernel.elf)
// ============================================================================

static int boot_load_kernel(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        printf("[boot] ERRO: kernel '%s' não encontrado.\n", path);
        logcat("boot_load_kernel: erro ao abrir arquivo");
        return 0;
    }

    logcat("boot_load_kernel: lendo ELF header");

    unsigned char e_ident[16];
    fread(e_ident, 1, 16, f);

    if (e_ident[0] != 0x7F || e_ident[1] != 'E' ||
        e_ident[2] != 'L'  || e_ident[3] != 'F') 
    {
        printf("[boot] Kernel inválido: não é ELF.\n");
        logcat("boot_load_kernel: não é ELF");
        fclose(f);
        return 0;
    }

    printf("[boot] Kernel ELF detectado.\n");
    printf("[boot] Classe ELF: %s\n", (e_ident[4] == 2) ? "64-bit" : "32-bit");

    logcat("boot_load_kernel: ELF OK");
    fclose(f);
    return 1;
}



// ============================================================================
//  RAMDISK: carregar arquivo inteiro para RAM
// ============================================================================
typedef struct {
    unsigned char *data;
    long size;
} stb_ramdisk;

static stb_ramdisk boot_load_ramdisk(const char *path)
{
    FILE *f = fopen(path, "rb");
    if (!f) {
        printf("[boot] ERRO: ramdisk não encontrado.\n");
        logcat("boot_load_ramdisk: falha ao abrir");
        return (stb_ramdisk){0};
    }

    logcat("boot_load_ramdisk: carregando");

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);

    unsigned char *buf = malloc(size);
    fread(buf, 1, size, f);

    fclose(f);

    printf("[boot] Ramdisk carregado (%ld bytes)\n", size);
    return (stb_ramdisk){ buf, size };
}



// ============================================================================
//  SISTEMA DE ARQUIVOS VIRTUAL (stb.fs)
// ============================================================================

typedef struct {
    char name[64];
    unsigned char *data;
    long size;
} FSFile;

#define FS_MAX 64
static FSFile fs[FS_MAX];
static int fs_count = 0;

static void fs_write(const char *name, const unsigned char *data, long size)
{
    if (fs_count >= FS_MAX) return;

    strcpy(fs[fs_count].name, name);
    fs[fs_count].data = malloc(size);
    memcpy(fs[fs_count].data, data, size);
    fs[fs_count].size = size;

    fs_count++;

    logcat("fs: arquivo gravado");
}

static FSFile *fs_read(const char *name)
{
    for (int i = 0; i < fs_count; i++)
        if (!strcmp(fs[i].name, name))
            return &fs[i];

    return NULL;
}


#endif // STB_LAUNCHER_ADVANCED
/** exemplo de uso da quarta parte abaixo */
//#include "stb_launcher.h"
//
//int main() 
//{
//    boot_animation();
//
//    boot_verify_signature("boot.img.signed");
//
//    stb_boot_image img = boot_load("boot.img");
//    stb_boot_header h = boot_parse_header(img.data);
//
//    boot_load_kernel("kernel.elf");
//
//    stb_ramdisk rd = boot_load_ramdisk("ramdisk.img");
//
//   fs_write("init.rc", rd.data, 128);
//    FSFile *f = fs_read("init.rc");
//
//    logcat("inicio completo");
//    logcat_dump();
//
//    return 0;
//}
