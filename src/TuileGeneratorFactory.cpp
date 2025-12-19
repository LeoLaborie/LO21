#include "TuileGeneratorFactory.h"

#include "TuileGenerator.h"

#include <memory>

std::unique_ptr<ITuileGenerator> DefaultTuileGeneratorFactory::creer(int nbJoueurs, bool varianteFullTuile) const
{
    // Par défaut on retourne l’implémentation standard, mais une autre factory
    // pourrait hériter de TuileGeneratorFactory et construire un générateur différent 
    return std::make_unique<TuileGenerator>(nbJoueurs, varianteFullTuile);
}
