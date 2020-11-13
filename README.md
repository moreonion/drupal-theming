[![Build Status](https://travis-ci.com/moreonion/drupal-theming.svg?branch=7.x-1.x)](https://travis-ci.com/moreonion/drupal-theming) [![codecov](https://codecov.io/gh/moreonion/drupal-theming/branch/7.x-1.x/graph/badge.svg)](https://codecov.io/gh/moreonion/drupal-theming)

# Theming

This is a Drupal module that replaces some Drupal core theme functions with more extensible and backwards compatible versions.

*This is a utility module. If no other module you want to have installed requires this you don’t need it.*


## Requirements

* Drupal 7.x


## Rationale

### Can’t this be handled in a theme?

No, themes can’t implement `hook_theme_registry_alter()` which is used for implementing the changes. Also having this in a module makes it possible for other modules to depend on it.

### Why don’t you patch Drupal core instead?

Getting such changes merged into Drupal 7.x was barely possible in 2012 (shortly after it was released). It’s near impossible now (2020).
