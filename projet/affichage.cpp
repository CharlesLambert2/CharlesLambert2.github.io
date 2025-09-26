#include "affichage.hpp"
#include "matrice.hpp"

Sdl::Sdl(const char* title, int width, int height)
    : window(nullptr), renderer(nullptr), en_cours(true) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Erreur d'initialisation de SDL: " << SDL_GetError() << std::endl;
        en_cours = false;
    } else {
        window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
        if (!window) {
            std::cerr << "Erreur de création de la fenêtre: " << SDL_GetError() << std::endl;
            en_cours = false;
        } else {
            renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
            if (!renderer) {
                std::cerr << "Erreur de création du renderer: " << SDL_GetError() << std::endl;
                en_cours = false;
            }
        }
    }
}

Sdl::~Sdl() {
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    SDL_Quit();
}

void Sdl::clearScreen(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    SDL_RenderClear(renderer);
}


void Sdl::present() {
    SDL_RenderPresent(renderer);
}

int Sdl::affichage_final(const std::vector<double>& C, const std::vector<double>& C_s, int WINDOW_WIDTH, int WINDOW_HEIGHT) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur d'initialisation de SDL : %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("Graphique SDL",
                                          SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                          WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        printf("Erreur de création de fenêtre : %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        printf("Erreur de création de rendu : %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Effacer l'écran
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Fond blanc
    SDL_RenderClear(renderer);

    // Déterminer les valeurs minimales et maximales pour le graphique
    double min_y = std::min(C[0], C[C.size()-1]);
    double max_y = std::max(C[0], C[C.size()-1]);

    // Décalage des axes
    const int X_OFFSET = 50; // Décalage de l'axe Y depuis le bord gauche
    const int Y_OFFSET = 50; // Décalage de l'axe X depuis le bord inférieur

    // Ajouter les axes
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255); // Blanc pour les axes

    // Axe X (horizontal, légèrement au-dessus du bord inférieur)
    int y_axis_position = WINDOW_HEIGHT - Y_OFFSET;
    SDL_RenderDrawLine(renderer, X_OFFSET, y_axis_position, WINDOW_WIDTH, y_axis_position);

    // Axe Y (vertical, légèrement à droite du bord gauche)
    int x_axis_position = X_OFFSET;
    SDL_RenderDrawLine(renderer, x_axis_position, 0, x_axis_position, WINDOW_HEIGHT - Y_OFFSET);

    // Ajouter des graduations sur l'axe X
    const int X_GRAD_INTERVAL = 50; // Intervalle entre chaque graduation sur l'axe X
    for (int x = X_OFFSET; x <= WINDOW_WIDTH; x += X_GRAD_INTERVAL) {
        SDL_RenderDrawLine(renderer, x, y_axis_position - 5, x, y_axis_position + 5); // Segment vertical
    }

    // Ajouter des graduations sur l'axe Y
    const int Y_GRAD_INTERVAL = 50; // Intervalle entre chaque graduation sur l'axe Y
    for (int y = 0; y <= WINDOW_HEIGHT - Y_OFFSET; y += Y_GRAD_INTERVAL) {
        SDL_RenderDrawLine(renderer, x_axis_position - 5, y, x_axis_position + 5, y); // Segment horizontal
    }

    // Dessiner la première courbe (en bleu)
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255); // Couleur bleue
    for (size_t s = 0; s < C.size() - 1; s++) {
        SDL_RenderDrawLine(renderer,
            X_OFFSET + (s * (WINDOW_WIDTH - X_OFFSET)) / (C.size() - 1),
            y_axis_position - ((C[s] - min_y) * (y_axis_position - 0) / (max_y - min_y)),
            X_OFFSET + ((s + 1) * (WINDOW_WIDTH - X_OFFSET)) / (C.size() - 1),
            y_axis_position - ((C[s+1] - min_y) * (y_axis_position - 0) / (max_y - min_y))
        );
    }

    // Dessiner la seconde courbe (en rouge)
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Couleur rouge
    for (size_t s = 0; s < C_s.size() -1; s++) {
        SDL_RenderDrawLine(renderer,
            X_OFFSET + (s * (WINDOW_WIDTH - X_OFFSET)) / (C_s.size() - 1),
            y_axis_position - ((C_s[s] - min_y) * (y_axis_position - 0) / (max_y - min_y)),
            X_OFFSET + ((s + 1) * (WINDOW_WIDTH - X_OFFSET)) / (C_s.size() - 1),
            y_axis_position - ((C_s[s+1] - min_y) * (y_axis_position - 0) / (max_y - min_y))
        );
    }

    // Afficher le rendu
    SDL_RenderPresent(renderer);

    // Attendre que l'utilisateur ferme la fenêtre
    SDL_Event e;
    int quit = 0;
    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
        }
    }

    // Libérer les ressources et fermer SDL
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

