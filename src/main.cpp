#include "Application.h"
#include "ConfigManager.h"
#include <cstdlib>
#include <SDL2/SDL.h>
#include "imgui.h"
#include "imgui_impl_sdl2.h"
#include "imgui_impl_sdlrenderer2.h"

bool runLauncher() {
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window* window = SDL_CreateWindow("Configuration de la Simulation", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 700, 900, SDL_WINDOW_SHOWN);
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_ACCELERATED);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
    ImGui_ImplSDLRenderer2_Init(renderer);

    GameConfig& config = ConfigManager::getInstance().getMutableConfig();
    bool startGame = false;
    bool keepRunning = true;

    while (keepRunning) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT) keepRunning = false;
        }

        ImGui_ImplSDLRenderer2_NewFrame();
        ImGui_ImplSDL2_NewFrame();
        ImGui::NewFrame();

        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2(700, 900));
        ImGui::Begin("Parametres de Generation", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoMove);

        // --- GENERATION PROCEDURALE ---
        if (ImGui::CollapsingHeader("Generation Procedurale (Terrain)")) {
            ImGui::Text("Bruit d'Altitude (Topologie)");
            // On limite la fréquence à de petites valeurs (ex: 0.001 à 0.05) pour éviter un monde "bruit blanc"
            ImGui::SliderFloat("Frequence Altitude", &config.altitudeFrequency, 0.001f, 0.05f, "%.4f");
            ImGui::SliderInt("Octaves (Details)", &config.altitudeOctaves, 1, 10);
            ImGui::SliderFloat("Lacunarite", &config.altitudeLacunarity, 1.0f, 4.0f);
            ImGui::SliderFloat("Gain (Attenuation)", &config.altitudeGain, 0.1f, 1.0f);

            ImGui::Separator();
            ImGui::Text("Bruits Climatiques & Sols");
            ImGui::SliderFloat("Frequence Temperature", &config.temperatureFrequency, 0.001f, 0.05f, "%.4f");
            ImGui::SliderFloat("Frequence Humidite", &config.humidityFrequency, 0.001f, 0.05f, "%.4f");
            ImGui::SliderFloat("Frequence Granularite", &config.granularFrequency, 0.01f, 0.2f, "%.4f");
        }

        // --- SECTION GENERALE & AGRICULTURE ---
        if (ImGui::CollapsingHeader("General & Agriculture", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::SliderFloat("Niveau de la mer", &config.seaLevel, -1.0f, 1.0f);
            ImGui::SliderFloat("Vitesse croissance ble", &config.wheatGrowthSpeed, 0.0f, 2.0f);
        }

        // --- HYDROLOGIE & EROSION ---
        if (ImGui::CollapsingHeader("Hydrologie & Erosion")) {
            ImGui::Text("Rivieres");
            ImGui::SliderFloat("Seuil debit riviere", &config.riverThreshold, 1.0f, 200.0f);
            ImGui::SliderFloat("Coefficient drop-off", &config.dropOff, 0.0f, 1.0f);
            
            ImGui::Separator();
            ImGui::Text("Erosion Hydraulique");
            ImGui::SliderFloat("K (Solubilite)", &config.K, 0.0f, 1.0f);
            ImGui::SliderFloat("M (Capacite)", &config.M, 0.0f, 1.0f);
            ImGui::SliderFloat("n (Vitesse)", &config.n, 0.0f, 2.0f);
            
            ImGui::Separator();
            ImGui::Text("Erosion Thermique");
            ImGui::SliderFloat("Angle de talus", &config.talusAngle, 0.0f, 0.1f, "%.4f");
            ImGui::SliderFloat("Friction", &config.friction, 0.0f, 1.0f);
            ImGui::InputInt("Iterations / tick", &config.iterationsPerTick);
        }

        // --- VEGETATION ---
        if (ImGui::CollapsingHeader("Vegetation")) {
            ImGui::SliderFloat("Chance pollinisation", &config.pollinizationChance, 0.0f, 1.0f);
            ImGui::SliderFloat("Facteur croissance", &config.growthFactor, 0.0f, 1.0f);
            ImGui::SliderFloat("Facteur mort", &config.deathFactor, 0.0f, 1.0f);
            ImGui::SliderInt("Rayon Cactus", &config.radiusCactus, 1, 10);
            ImGui::SliderInt("Rayon Pin", &config.radiusPineTree, 1, 10);
            ImGui::SliderInt("Rayon Chene", &config.radiusOakTree, 1, 15);
            ImGui::SliderInt("Rayon Jungle", &config.radiusJungleTree, 1, 15);
        }

        // --- MINERAUX ---
        if (ImGui::CollapsingHeader("Mineraux")) {
            ImGui::SliderFloat("Frequence du bruit", &config.noiseFrequency, 0.01f, 0.5f);
            ImGui::SliderFloat("Chance spawn de base", &config.baseSpawnChance, 0.0f, 0.2f);
            ImGui::SliderFloat("Chance initiale filon", &config.veinInitialChance, 0.0f, 1.0f);
            ImGui::SliderFloat("Decroissance filon", &config.veinDecayRate, 0.0f, 1.0f);
            
            if (ImGui::TreeNode("Details par ressource")) {
                static const char* resNames[] = {"Charbon", "Fer", "Or", "Bronze"};
                float* minAlts[] = {&config.coalMinAltitude, &config.ironMinAltitude, &config.goldMinAltitude, &config.bronzeMinAltitude};
                float* maxAlts[] = {&config.coalMaxAltitude, &config.ironMaxAltitude, &config.goldMaxAltitude, &config.bronzeMaxAltitude};
                float* rarities[] = {&config.coalRarity, &config.ironRarity, &config.goldRarity, &config.bronzeRarity};

                for(int i=0; i<4; i++) {
                    if (ImGui::TreeNode(resNames[i])) {
                        ImGui::SliderFloat("Altitude Min", minAlts[i], 0.0f, 1.0f);
                        ImGui::SliderFloat("Altitude Max", maxAlts[i], 0.0f, 1.0f);
                        ImGui::SliderFloat("Rarete", rarities[i], 0.0f, 1.0f);
                        ImGui::TreePop();
                    }
                }
                ImGui::TreePop();
            }
        }

        // --- CIVILISATION & VILLES ---
        if (ImGui::CollapsingHeader("Civilisation & Villes")) {
            ImGui::InputInt("Nombre de factions", &config.numberOfFactions);
            ImGui::InputInt("Population initiale", &config.populationTotale);
            ImGui::InputInt("Distance min entre capitales", &config.minDistance);
            ImGui::SliderInt("Capacite accueil base", &config.carryingCapacityBase, 100, 5000);
            ImGui::SliderFloat("Taux de croissance", &config.growthRate, 0.0f, 1.0f);
            ImGui::InputInt("Stock nourriture initial", &config.stockFood);
        }

        // --- GESTION DES RESSOURCES ET TECH ---
        if (ImGui::CollapsingHeader("Economie & Technologie")) {
            ImGui::Text("Ressources");
            ImGui::SliderFloat("Vitesse abattage (Bois)", &config.treeChopRate, 0.0f, 1.0f);
            ImGui::SliderFloat("Vitesse minage (Minerai)", &config.miningRate, 0.0f, 1.0f);
            
            ImGui::Separator();
            ImGui::Text("Technologie");
            ImGui::InputInt("Cout Bois -> Bronze", &config.costWoodBronze);
            ImGui::InputInt("Pop requise Bronze", &config.reqPopBronze);
            ImGui::InputInt("Cout Bronze -> Fer", &config.costBronzeIron);
            ImGui::InputInt("Cout Charbon -> Fer", &config.costCoalIron);
        }

        // --- GUERRE ET TENSION ---
        if (ImGui::CollapsingHeader("Warfare & Territoire")) {
            ImGui::SliderFloat("Taux pertes combat", &config.baseCasualtyRate, 0.0f, 1.0f);
            ImGui::SliderInt("Bonus portee combat", &config.combatRangeBonus, 0, 5);
            ImGui::SliderInt("Facteur tension", &config.tensionFactor, 0, 50);
        }

        

        ImGui::Spacing();
        ImGui::Separator();
        if (ImGui::Button("SAUVEGARDER ET LANCER LA SIMULATION", ImVec2(-1, 60))) {
            ConfigManager::getInstance().saveConfig("config.json", "config/");
            startGame = true;
            keepRunning = false;
        }

        ImGui::End();
        ImGui::Render();
        SDL_SetRenderDrawColor(renderer, 20, 20, 20, 255);
        SDL_RenderClear(renderer);
        ImGui_ImplSDLRenderer2_RenderDrawData(ImGui::GetDrawData(), renderer);
        SDL_RenderPresent(renderer);
    }

    // Nettoyage ImGui & SDL locale
    ImGui_ImplSDLRenderer2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    
    return startGame;
}

int main() {
    srand(time(nullptr));
    ConfigManager::getInstance().loadConfig("config.json", "config/");

    if(!runLauncher()) {
        return 0; // fermeture du launcher, pas de lancement de l'application
    }
    int seed  = rand()%1000;
    Application app(1720,1080, 500, 500, 55);
    app.run();
    return 0;
}